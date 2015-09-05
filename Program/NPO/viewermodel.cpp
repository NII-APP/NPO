#include "viewermodel.h"
#include <fem.h>
#include <QModelIndex>
#include "identity.h"
#include "application.h"
#include "project.h"

ViewerModel::ViewerModel(Project * p, QObject* parent)
    : QAbstractItemModel(parent)
    , __project(p)
{
    Q_ASSERT(sizeof(quintptr) >= 8);
}

bool ViewerModel::isRootIndex(const QModelIndex& i) {
    return i.row() == -1 && i.column() == -1;
}
bool ViewerModel::isTopIndex(const QModelIndex& i) {
    return !i.internalId();
}
bool ViewerModel::isInfoIndex(const QModelIndex& i) {
    return i.internalId() && !(i.internalId() >> 32);
}
ViewerModel::ModelRow ViewerModel::modelRole(const int row, const int model) const {
    switch (row) {
    case 0:
        return Vertexes;
    case 1:
        return MAC;
    case 2:
        try {
            return __project->modelsList().at(model)->getModes().empty() ? ImportModes : Modes;
        } catch (...) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << model << "value fail";
#endif
            return WrongId;
        }
    case 3:
        try {
            return __project->modelsList().at(model)->getModes().empty() ? ModesIdentification : WrongId;
        } catch (...) {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << model << "value fail two";
#endif
            return WrongId;
        }
    }
    return WrongId;
}

ViewerModel::ModelRow ViewerModel::modelRole(const QModelIndex& i) const {
    if (i.internalId() >> 32) {
        return static_cast<ModelRow>(i.internalId() >> 32);
    } else if (isInfoIndex(i)) {
        return modelRole(i.row(), modelId(i));
    }
    return WrongId;
}

int ViewerModel::toRow(ModelRow r) {
    switch (r) {
    case Vertexes:
        return 0;
    case MAC:
        return 1;
    case ImportModes: case Modes:
        return 2;
    case ModesIdentification: default:
        return 3;
    }
}

int ViewerModel::modelId(const QModelIndex& i) {
    if (isRootIndex(i)) {
        return -1;
    }
    return isTopIndex(i) ? i.row() : (i.internalId() & 0xFFFFFFFF) - 1;
}

QModelIndex ViewerModel::index(int row, int column, const QModelIndex &parent) const {
#ifdef VIEWERMODEL_DEBUG
    qDebug() << "index" << row << parent;
#endif
    if (isRootIndex(parent)) {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << "\ttopLevel" << createIndex(row, column, (quintptr)0);
#endif
        //if it's a first-level item the number of model is a row
        return createIndex(row, column, (quintptr)0);
    } else
        //otherwice innerId is the black magic!
        if (isInfoIndex(parent)) {
        //first 4 bytes is a row of first information level. second 4 bytes is a model index
            return createIndex(row, column, parent.internalId() | (static_cast<quintptr>(modelRole(parent.row(), modelId(parent))) << 32));
    } else {
        //first 4 bytes is a null, second 4 bytes still a model index
        return createIndex(row, column, parent.row() + 1);
    }
}



QModelIndex	ViewerModel::parent(const QModelIndex & index) const {
#ifdef VIEWERMODEL_DEBUG
    qDebug() << "parent" << index;
#endif
    if (isRootIndex(index) || isTopIndex(index)) {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << "\troot" << index;
#endif
        return QModelIndex();
    } else if (isInfoIndex(index)) {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << "\ttop" << createIndex(modelId(index), 0);
#endif
        return createIndex(modelId(index), 0);
    } else {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << "\tinfo" << createIndex(toRow(modelRole(index)), 0, index.internalId() & 0xFFFFFFFF);
#endif
        return createIndex(toRow(modelRole(index)), 0, index.internalId() & 0xFFFFFFFF);
    }
}

int	ViewerModel::columnCount(const QModelIndex &) const { return 1; }
int	ViewerModel::rowCount(const QModelIndex & i) const {
#ifdef VIEWERMODEL_DEBUG
    qDebug() << "rowCount" << i;
#endif
    if (isRootIndex(i)) {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << "\troot" << static_cast<int>(__project->modelsList().size() + 1);
#endif
        return static_cast<int>(__project->modelsList().size() + 1);
    }
    if (isTopIndex(i)) {
        try {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "\ttop" <<  (__project->modelsList().at(i.row())->getModes().empty() ? WithoutModes : WithModes);
#endif
            return __project->modelsList().at(i.row())->getModes().empty() ? WithoutModes : WithModes;
        } catch (...) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "\ttopReject";
#endif
            return 0;
        }
    }
    if (!isInfoIndex(i)) {
        return 0;
    }
    switch (modelRole(i)) {
    case Modes:
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "\tmodes";
#endif
        try {
            return static_cast<int>(__project->modelsList().at(modelId(i))->getModes().size());
        } catch (...) {
            return 0;
        }

    default:
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "\tenother";
#endif
        return 0;
    }
}

QVariant ViewerModel::data(const QModelIndex & index, int role) const {
    if (role != Qt::DisplayRole) {
        if (role == Qt::DecorationRole && isTopIndex(index) && __project->modelsList().size() == index.row()) {
            static const QIcon add(Identity::fromSvg(":/media/images/list-add-512px.svg"));
            return add;
        }
        return QVariant();
    }
#ifdef VIEWERMODEL_DEBUG
    qDebug() << "data" << index;
#endif
    if (isTopIndex(index)) {
        try {
            return __project->modelsList().at(index.row())->getName();
        } catch(...) {
            return Application::identity()->geometriesModelAdd();
        }
    }
    try {
        const FEM* const mesh(__project->modelsList().at(modelId(index)));
        if (isInfoIndex(index)) {
            ModelRow r(modelRole(index));
                return Application::identity()->vieverModelValues(r, static_cast<int>(r == Modes ? mesh->getModes().size() : mesh->getNodes().size()));
        }
        return Application::identity()->formSelectorLabel().arg(QString::number(index.row() + 1), QString::number(mesh->getModes().at(index.row()).frequency()));
    } catch (...) {
        return "Model id fail" + QString::number(modelId(index));
    }
}
