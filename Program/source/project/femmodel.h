#ifndef FEMMODEL_H
#define FEMMODEL_H

#include <QAbstractItemModel>
#include <memory>
#include <functional>

#include "kernel/igofesolver.h"
class Project;
class FEM;
class EigenModesFounder;

class FEMModel : public QAbstractItemModel
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
    typedef std::shared_ptr<EigenModesFounder> SharedEigenModesFounder;

private:
    enum ModelRowsCount {
        WithModes = 3,
        WithoutModes = 4
    };

    EigenModesFounder* provideFounder(int modelId);
    SharedEigenModesFounder foundModes(const int modelId, std::function<void(EigenModesFounder* const)> f);

public:
    static bool isRootIndex(const QModelIndex&);
    static bool isTopIndex(const QModelIndex&);
    static bool isInfoIndex(const QModelIndex&);
    ModelRow modelRole(const QModelIndex&) const;
    ModelRow modelRole(const int row, const int model) const;
    static int modelId(const QModelIndex&);
    static int toRow(ModelRow);

    FEMModel(const Project*, QObject* parent = 0);

    QModelIndex FEMIndex(int femId) const;
    QModelIndex FEMIndex(const FEM *) const;
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

    SharedEigenModesFounder importModes(const int modelId, const QString& fileName);
    SharedEigenModesFounder calculateModes(const int modelId, const IgoFESolver::SolverOptions &options);
    SharedEigenModesFounder identificateModes(const int modelId);

    void setProject(const Project*);
    const Project* getProject();
    void beginAddModes(const QModelIndex&);
    void endAddModes(const QModelIndex&);
    void beginAddModes(const FEM*);
    void endAddModes(const FEM*);
private:
    const Project* __project;
};

#endif // FEMMODEL_H
