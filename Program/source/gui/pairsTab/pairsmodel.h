#ifndef PAIRSMODEL_H
#define PAIRSMODEL_H

#include <QAbstractItemModel>

class Project;
class FEMPair;

class PairModel : public QAbstractItemModel
{
    Q_OBJECT

    static bool isRootIndex(const QModelIndex& i);
public:
    explicit PairModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex index(int row, int column, const QModelIndex &) const;

    void setProject(const Project* const);
    const Project* getProject() const;

    static int pairId(const QModelIndex &);

    void insertPair(FEMPair * const);

private:
    const Project* __project;
};

#endif // PAIRSMODEL_H
