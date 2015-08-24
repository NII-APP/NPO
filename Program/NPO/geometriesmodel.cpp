#include "geometriesmodel.h"
#include "application.h"
#include "project.h"
#include "identity.h"
#include "fem.h"

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
    return QVariant(QString::number(index.row()));
}

FEM * GeometriesModel::mesh(const QModelIndex &index) {
    return 0;
}
