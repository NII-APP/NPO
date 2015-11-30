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
#ifdef PAIRS_MODEL_DEBUG
    qDebug() << "rowCount" << parent;
#endif
    return QModelIndex() != parent ? 0 : (__project ? static_cast<int>(__project->pairsList().size() + 1) : 1);
}
int PairModel::columnCount(const QModelIndex &) const { return 1; }

QVariant PairModel::data(const QModelIndex &index, int role) const {
#ifdef PAIRS_MODEL_DEBUG
        qDebug() << "data" << index;
#endif
    if (__project == nullptr) {
        return role == Qt::DisplayRole ? Application::identity()->tr("project is null", "pair tab/model") : QVariant();
    }
    if (role == Qt::DisplayRole) {
        return index.row() < __project->pairsList().size() ? Application::identity()->tr("item", "pair tab/model").arg(index.row() + 1)
                                                           : Application::identity()->tr("add item", "pair tab/model");
    } else if (role == Qt::DecorationRole && __project->pairsList().size() == index.row()) {
        static const QIcon add(Application::identity()->icon("add item"));
        return add;
    }
    return QVariant();
}

QModelIndex PairModel::parent(const QModelIndex &) const {
#ifdef PAIRS_MODEL_DEBUG
    qDebug() << "parent" << child;
#endif
    return QModelIndex();
}

QModelIndex PairModel::index(int row, int column, const QModelIndex &) const {
#ifdef PAIRS_MODEL_DEBUG
    qDebug() << "index" << row << column << parent;
#endif
    return this->createIndex(row, column);
}

void PairModel::setProject(const Project* const p) { __project = p; }
const Project* PairModel::getProject() const { return __project; }
