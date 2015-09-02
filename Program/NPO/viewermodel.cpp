#include "viewermodel.h"
#include <fem.h>
#include <QModelIndex>
#include "identity.h"
#include "application.h"
#include "project.h"

//#define NO_VIEWERMODEL_DEBUG

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
ViewerModel::ModelRow ViewerModel::modelRole(const QModelIndex& i) const {
    switch (i.internalId() >> 32 ? i.internalId() & 0xFFFFFFFF : i.row()) {
    case 0:
        return Vertexes;
    case 1:
        return MAC;
    case 2:
        return __project->modelsList().at(modelId(i))->getModes().empty() ? ImportModes : Modes;
    case 3:
        return ModesIdentification;
    }
    return WrongId;
}
int ViewerModel::modelId(const QModelIndex& i) {
    return isTopIndex(i) ?
                i.row() :
                ((i.internalId() >> 32 ?
                     i.internalId() >> 32 :
                     i.internalId()) - 1);
}

QModelIndex ViewerModel::index(int row, int column, const QModelIndex &parent) const {
#ifndef NO_VIEWERMODEL_DEBUG
    qDebug() << "index" << row << parent;
#endif
    if (isRootIndex(parent)) {
#ifndef NO_VIEWERMODEL_DEBUG
        qDebug() << "\ttopLevel" << createIndex(row, column, (quintptr)0);
#endif
        //if it's a first-level item the number of model is a row
        return createIndex(row, column, (quintptr)0);
    }
    //innerId is the dark magic!
    //when it's a first level of model information it's just a model id + 1
    //when it's a second level of model information the first 4 bytes is a model index. second 4 bytes is a row of first information level
#ifndef NO_VIEWERMODEL_DEBUG
    qDebug() << "\tinner"
             << static_cast<quintptr>(parent.internalId() << 32) << static_cast<quintptr>(parent.row() + 1);
#endif
    return createIndex(row, column, static_cast<quintptr>(parent.internalId() << 32) | static_cast<quintptr>(parent.row() + 1));
}

QModelIndex	ViewerModel::parent(const QModelIndex & index) const {
#ifndef NO_VIEWERMODEL_DEBUG
    qDebug() << "parent" << index;
#endif
    if (isRootIndex(index) || isTopIndex(index)) {
        return QModelIndex();
    }
    if (isInfoIndex(index)) {
        return createIndex(index.internalId(), 0);
    }
    return createIndex(index.internalId() >> 32, 0);
}

int	ViewerModel::columnCount(const QModelIndex &) const { return 1; }
int	ViewerModel::rowCount(const QModelIndex & i) const {
#ifndef NO_VIEWERMODEL_DEBUG
    qDebug() << "rowCount" << i;
#endif
    if (isRootIndex(i)) {
#ifndef NO_VIEWERMODEL_DEBUG
        qDebug() << "\troot" << static_cast<int>(__project->modelsList().size() + 1);
#endif
        return static_cast<int>(__project->modelsList().size() + 1);
    }
    if (isTopIndex(i)) {
        try {
#ifndef NO_VIEWERMODEL_DEBUG
            qDebug() << "\ttop" <<  (__project->modelsList().at(i.row())->getModes().empty() ? WithoutModes : WithModes);
#endif
            return __project->modelsList().at(i.row())->getModes().empty() ? WithoutModes : WithModes;
        } catch (...) {
#ifndef NO_VIEWERMODEL_DEBUG
            qDebug() << "\ttopReject";
#endif
            return 0;
        }
    }
    switch (modelRole(i)) {
    case Modes:
#ifndef NO_VIEWERMODEL_DEBUG
            qDebug() << "\tmodes";
#endif
        return static_cast<int>(__project->modelsList().at(modelId(i))->getModes().size());
    default:
#ifndef NO_VIEWERMODEL_DEBUG
            qDebug() << "\tenother";
#endif
        return 0;
    }
}

QVariant ViewerModel::data(const QModelIndex & index, int role) const {
#ifndef NO_VIEWERMODEL_DEBUG
    qDebug() << "data";
#endif
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (isTopIndex(index)) {
        try {
            return __project->modelsList().at(index.row())->getName();
        } catch(...) {
            return Application::identity()->geometriesModelAdd();
        }
    }
    return QString::number(index.row());
    if (isInfoIndex(index)) {
        ModelRow r(modelRole(index));
        const FEM* const mesh(__project->modelsList().at(index.row()));
        return Application::identity()->vieverModelValues(r, static_cast<int>(r == Modes ? mesh->getModes().size() : mesh->getNodes().size()));
    }
    return Application::identity()->formSelectorLabel();
}
