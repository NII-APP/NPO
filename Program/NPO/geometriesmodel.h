#ifndef GEOMETRIESMODEL_H
#define GEOMETRIESMODEL_H

#include <QAbstractListModel>
#include <QVariant>
class FEM;

class GeometriesModel : public QAbstractListModel
{
public:
    GeometriesModel(QObject *parent = 0);
    ~GeometriesModel();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    static FEM* mesh(const QModelIndex &index);
};

#endif // GEOMETRIESMODEL_H
