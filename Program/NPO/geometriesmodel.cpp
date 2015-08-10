#include "geometriesmodel.h"
#include "application.h"
#include "project.h"
#include "identity.h"
#include "eigenmodes.h"

GeometriesModel::GeometriesModel(QObject* parent)
    : QAbstractListModel(parent)
{

}

GeometriesModel::~GeometriesModel()
{

}

int GeometriesModel::rowCount(const QModelIndex &) const {
    return static_cast<int>(Application::project()->modelsList().size()) + 1;
}

QVariant GeometriesModel::data(const QModelIndex &index, int role) const {
    if (Qt::DisplayRole != role) {
        return QVariant();
    }
    if (index.row() >= Application::project()->modelsList().size()) {
        return Application::identity()->geometriesModelAdd();
    }
    return QVariant(mesh(index)->getName());
}

MeshForm* GeometriesModel::mesh(const QModelIndex &index) {
    if (index.row() >= Application::project()->modelsList().size() || index.row() < 0) {
        return 0;
    } else {
        return static_cast<MeshForm*>(Application::project()->modelsList().at(index.row()));
    }
}
