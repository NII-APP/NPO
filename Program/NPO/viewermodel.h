#ifndef VIEWERMODEL_H
#define VIEWERMODEL_H

#include <QAbstractItemModel>

class Project;

class ViewerModel : public QAbstractItemModel
{
public:
    enum ModelRow {
        Vertexes = 1,
        Modes = 2,
        ImportModes = 3,
        ModesIdentification = 4,
        MAC = 5,

        WrongId = 0xFFFF
    };

private:
    enum ModelRowsCount {
        WithModes = 3,
        WithoutModes = 4
    };
    static bool isRootIndex(const QModelIndex&);
    static bool isTopIndex(const QModelIndex&);
    static bool isInfoIndex(const QModelIndex&);
    ModelRow modelRole(const QModelIndex&) const;
    static int modelId(const QModelIndex&);

public:
    ViewerModel(Project*, QObject* parent = 0);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex	parent(const QModelIndex & index) const;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
private:
    Project* __project;
};

#endif // VIEWERMODEL_H
