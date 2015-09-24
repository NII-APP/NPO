#include "pairsmodel.h"

#include <QDebug>

#include "project.h"
#include "application.h"
#include "identity.h"

PairModel::PairModel(QObject *parent)
    : QAbstractItemModel(parent)
    , __project(nullptr)
{

}

int PairModel::rowCount(const QModelIndex &parent) const {
    qDebug() << "rowCount" << parent;
    return QModelIndex() != parent ? 0 : (__project ? __project->pairsList().size() + 1 : 1);
}
int PairModel::columnCount(const QModelIndex &parent) const { return 1; }

QVariant PairModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        qDebug() << "data" << index;
        if (__project == nullptr) {
            return Application::identity()->tr("project is null", "pair tab/model");
        }
        return index.row() < __project->pairsList().size() ? Application::identity()->tr("item", "pair tab/model").arg(index.row() + 1)
                                                           : Application::identity()->tr("add item", "pair tab/model");
    }
    return QVariant();
}

QModelIndex PairModel::parent(const QModelIndex &child) const {
    qDebug() << "parent" << child;
    return QModelIndex();
}

QModelIndex PairModel::index(int row, int column, const QModelIndex &parent) const {
    qDebug() << "index" << row << column << parent;
    return this->createIndex(row, column);
}

void PairModel::setProject(const Project* p) { p = __project; }
const Project* PairModel::getProject() const { return __project; }
