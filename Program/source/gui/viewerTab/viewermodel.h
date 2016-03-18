#ifndef VIEWERMODEL_H
#define VIEWERMODEL_H

#include <QAbstractItemModel>

class Project;
class FEM;
class FEMProcessor;

class ViewerModel : public QAbstractItemModel
{
public:
    enum ModelRow {
        Vertexes = 1,
        Modes = 2,
        ImportModes = 3,
        ModesIdentification = 4,
        ModesCompute = 5,
        MAC = 6,

        WrongId = 0xFFFF
    };

private:
    enum ModelRowsCount {
        WithModes = 3,
        WithoutModes = 4
    };
public:
    static bool isRootIndex(const QModelIndex&);
    static bool isTopIndex(const QModelIndex&);
    static bool isInfoIndex(const QModelIndex&);
    ModelRow modelRole(const QModelIndex&) const;
    ModelRow modelRole(const int row, const int model) const;
    static int modelId(const QModelIndex&);
    static int toRow(ModelRow);

    ViewerModel(const Project*, QObject* parent = 0);

    QModelIndex FEMIndex(int femId);
    QModelIndex FEMIndex(FEM*);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex	parent(const QModelIndex & index) const;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool removeFEM(int id);
    void addFEM(const QString&);
    FEMProcessor* importModes(int, const QString&);

    void setProject(const Project*);
    const Project* getProject();
private:
    const Project* __project;
};

#endif // VIEWERMODEL_H
