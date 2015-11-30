#ifndef PAIRSMODEL_H
#define PAIRSMODEL_H

#include <QAbstractItemModel>

class Project;

class PairModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit PairModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex index(int row, int column, const QModelIndex &) const;

    void setProject(const Project* const);
    const Project* getProject() const;

private:
    const Project* __project;
};

#endif // PAIRSMODEL_H
