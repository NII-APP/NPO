#ifndef GEOMETRIESMODEL_H
#define GEOMETRIESMODEL_H

#include <QAbstractListModel>
#include <QVariant>

class GeometriesModel : public QAbstractListModel
{
public:
    GeometriesModel(QObject *parent = 0);
    ~GeometriesModel();

    int rowCount(const QModelIndex &parent) const { return 0; }
    QVariant data(const QModelIndex &index, int role) const { return QVariant(); }
};

#endif // GEOMETRIESMODEL_H
