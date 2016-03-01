#include "viewermodel.h"
#include <fem.h>
#include <QModelIndex>
#include "identity.h"
#include "application.h"
#include "project.h"

ViewerModel::ViewerModel(const Project * p, QObject* parent)
    : QAbstractItemModel(parent)
    , __project(p)
{
    Q_ASSERT(sizeof(quintptr) >= 4);
}

bool ViewerModel::isRootIndex(const QModelIndex& i) {
    return i.row() == -1 && i.column() == -1 && !i.internalId();
}
bool ViewerModel::isTopIndex(const QModelIndex& i) {
    return !i.internalId() && !isRootIndex(i);
}
bool ViewerModel::isInfoIndex(const QModelIndex& i) {
    return i.internalId() && !(i.internalId() >> 16);
}
ViewerModel::ModelRow ViewerModel::modelRole(const int row, const int model) const {
    switch (row) {
    case 0:
        return Vertexes;
    case 1:
        try {
            return __project->modelsList().at(model)->getModes().empty() ? ImportModes : MAC;
        } catch (...) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << model << "value fail";
#endif
            return WrongId;
        }
    case 2:
        try {
            return __project->modelsList().at(model)->getModes().empty() ? ModesIdentification : Modes;
        } catch (...) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << model << "value fail";
#endif
            return WrongId;
        }
    case 3:
        try {
            return __project->modelsList().at(model)->getModes().empty() ? ModesCompute : WrongId;
        } catch (...) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << model << "value fail";
#endif
            return WrongId;
        }
    }
    return WrongId;
}

ViewerModel::ModelRow ViewerModel::modelRole(const QModelIndex& i) const {
    if (i.internalId() >> 16) {
        return static_cast<ModelRow>(i.internalId() >> 16);
    } else if (isInfoIndex(i)) {
        return modelRole(i.row(), modelId(i));
    }
    return WrongId;
}

int ViewerModel::toRow(ModelRow r) {
    switch (r) {
    case Vertexes:
        return 0;
    case MAC: case ImportModes:
        return 1;
    case Modes: case ModesIdentification: default:
        return 2;
    }
}

int ViewerModel::modelId(const QModelIndex& i) {
    if (isRootIndex(i)) {
        return -1;
    }
    return isTopIndex(i) ? i.row() : (i.internalId() & 0xFFFF) - 1;
}

QModelIndex ViewerModel::index(int row, int column, const QModelIndex &parent) const {
#ifdef VIEWERMODEL_DEBUG
    qDebug() << "index" << row << parent << parent.internalId();
#endif
    if (isRootIndex(parent)) {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << "\ttopLevel" << createIndex(row, column, (quintptr)0);
#endif
        //if it's a first-level item the number of model is a row
        return createIndex(row, column, (quintptr)0);
    } else {
        //otherwice innerId is the black magic!
        //last 4 bytes separated for two parts: first two bytes is a row in model information, last two - model id
        if (isInfoIndex(parent)) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "create second depance lavel" << parent << QString::number(parent.internalId() | (static_cast<quintptr>(modelRole(parent.row(), modelId(parent))) << 16), 16);
#endif
            return createIndex(row, column, parent.internalId() | (static_cast<quintptr>(modelRole(parent.row(), modelId(parent))) << 16));
        } else {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "create first depance lavel" << parent.row() + 1 << parent;
#endif
            return createIndex(row, column, parent.row() + 1);
        }
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
        qDebug() << "\tinfo" << createIndex(toRow(modelRole(index)), 0, index.internalId() & 0xFFFF);
#endif
        return createIndex(toRow(modelRole(index)), 0, index.internalId() & 0xFFFF);
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
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "\tinModes" << 0;
#endif
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

QVariant ViewerModel::headerData(int, Qt::Orientation, int role) const {
    if (role == Qt::DisplayRole) {
        return Application::identity()->tr("models list", "viewer model");
    }
    return QVariant();
}

QVariant ViewerModel::data(const QModelIndex & index, int role) const {
    if (role != Qt::DisplayRole) {
        if (role == Qt::DecorationRole && isTopIndex(index) && __project->modelsList().size() == index.row()) {
            static const QIcon add(Identity::fromSvg(":/media/images/list-add-512px.svg"));
            return add;
        }
        return QVariant();
    }
    if (isRootIndex(index)) {
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
            static const QString vertexesStr(Application::identity()->tr("vertexes", "viewer model"));
            static const QString modesStr(Application::identity()->tr("modes", "viewer model"));
            static const QString importStr(Application::identity()->tr("import", "viewer model"));
            static const QString identificationStr(Application::identity()->tr("identification", "viewer model"));
            static const QString compute(Application::identity()->tr("compute", "viewer model"));
            static const QString macStr(Application::identity()->tr("mac", "viewer model"));
            switch (r) {
            case ViewerModel::Vertexes:
                return vertexesStr.arg(mesh->getNodes().length());
            case ViewerModel::Modes:
                return modesStr.arg(mesh->getModes().size());
            case ViewerModel::ImportModes:
                return importStr;
            case ViewerModel::ModesIdentification:
                return identificationStr;
            case ViewerModel::ModesCompute:
                return compute;
            case ViewerModel::MAC:
                return macStr;
            }
            return "something wrong";
        }
        return Application::identity()->formSelectorLabel().arg(QString::number(index.row() + 1), QString::number(mesh->getModes().at(index.row()).frequency()));
    } catch (...) {
        qFatal((QString("model id fail 0x") + QString::number(index.internalId(), 16) + ' ' + QString::number(modelId(index))).toLocal8Bit());
        return "Model id fail" + QString::number(modelId(index));
    }
}

void ViewerModel::setProject(const Project* p) {
    __project = p;
}

const Project* ViewerModel::getProject() {
    return __project;
}
