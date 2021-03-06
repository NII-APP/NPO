#include "femmodel.h"

#include <cassert>
#include <exception>

#include <QModelIndex>
#include <QDialog>

#include <fem.h>
#include "identity.h"
#include "application.h"
#include "project.h"
#include "kernel/eigenmodesfounder.h"
#include "gui/mainWindow/mainwindow.h"


FEMModel::FEMModel(const Project * p, QObject* parent)
    : QAbstractItemModel(parent)
    , __project(p)
{
    Q_ASSERT(sizeof(quintptr) >= 4);
}

bool FEMModel::isRootIndex(const QModelIndex& i) {
    return i.row() == -1 && i.column() == -1 && !i.internalId();
}
bool FEMModel::isTopIndex(const QModelIndex& i) {
    return !i.internalId() && !isRootIndex(i);
}
bool FEMModel::isInfoIndex(const QModelIndex& i) {
    return i.internalId() && !(i.internalId() >> 16);
}
FEMModel::ModelRow FEMModel::modelRole(const int row, const int model) const {
    switch (row) {
    case 0:
        return Vertexes;
    case 1:
        try {
            return __project->FEMList().at(model)->getModes().empty() ? ImportModes : MAC;
        } catch (...) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << model << "value fail";
#endif
            return WrongId;
        }
    case 2:
        try {
            return __project->FEMList().at(model)->getModes().empty() ? ModesIdentification : Modes;
        } catch (...) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << model << "value fail";
#endif
            return WrongId;
        }
    case 3:
        try {
            return __project->FEMList().at(model)->getModes().empty() ? ModesCompute : WrongId;
        } catch (...) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << model << "value fail";
#endif
            return WrongId;
        }
    }
    return WrongId;
}

FEMModel::ModelRow FEMModel::modelRole(const QModelIndex& i) const {
    if (i.internalId() >> 16) {
        return static_cast<ModelRow>(i.internalId() >> 16);
    } else if (isInfoIndex(i)) {
        return modelRole(i.row(), modelId(i));
    }
    return WrongId;
}

int FEMModel::toRow(ModelRow r) {
    switch (r) {
    case Vertexes:
        return 0;
    case MAC: case ImportModes:
        return 1;
    case Modes: case ModesIdentification: default:
        return 2;
    }
}

int FEMModel::modelId(const QModelIndex& i) {
    if (isRootIndex(i)) {
        return -1;
    }
    return isTopIndex(i) ? i.row() : (i.internalId() & 0xFFFF) - 1;
}

Qt::ItemFlags FEMModel::flags(const QModelIndex &index) const
{
    if (isTopIndex(index)) {
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }
    return QAbstractItemModel::flags(index);
}

QModelIndex FEMModel::index(int row, int column, const QModelIndex &parent) const {
#ifdef VIEWERMODEL_DEBUG
    qDebug() << "index" << row << parent << parent.internalId();
#endif
    if (isRootIndex(parent)) {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << "\ttopLevel" << createIndex(row, column, (quintptr)0);
#endif
        //if it's a first-level item the number of model is a row
        return createIndex(row, column, (quintptr)0);
    } else {
        //otherwice innerId is the black magic!
        //last 4 bytes separated for two parts: first two bytes is a row in model information, last two - model id
        if (isInfoIndex(parent)) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "create second depance lavel" << parent << QString::number(parent.internalId() | (static_cast<quintptr>(modelRole(parent.row(), modelId(parent))) << 16), 16);
#endif
            return createIndex(row, column, parent.internalId() | (static_cast<quintptr>(modelRole(parent.row(), modelId(parent))) << 16));
        } else {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "create first depance lavel" << parent.row() + 1 << parent;
#endif
            return createIndex(row, column, parent.row() + 1);
        }
    }
}

QModelIndex FEMModel::FEMIndex(int femId) const
{
    return index(femId, 0, QModelIndex());
}

QModelIndex FEMModel::FEMIndex(const FEM* femId) const
{
    return index(__project->toId(femId), 0, QModelIndex());
}

QModelIndex	FEMModel::parent(const QModelIndex & index) const {
#ifdef VIEWERMODEL_DEBUG
    qDebug() << "parent" << index;
#endif
    if (isRootIndex(index) || isTopIndex(index)) {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << "\troot" << index;
#endif
        return QModelIndex();
    } else if (isInfoIndex(index)) {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << "\ttop" << createIndex(modelId(index), 0);
#endif
        return createIndex(modelId(index), 0);
    } else {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << "\tinfo" << createIndex(toRow(modelRole(index)), 0, index.internalId() & 0xFFFF);
#endif
        return createIndex(toRow(modelRole(index)), 0, index.internalId() & 0xFFFF);
    }
}

int	FEMModel::columnCount(const QModelIndex &) const { return 1; }
int	FEMModel::rowCount(const QModelIndex & i) const {
#ifdef VIEWERMODEL_DEBUG
    qDebug() << "rowCount" << i;
#endif
    if (isRootIndex(i)) {
#ifdef VIEWERMODEL_DEBUG
        qDebug() << "\troot" << static_cast<int>(__project->FEMList().size() + 1);
#endif
        return static_cast<int>(__project->FEMList().size() + 1);
    }
    if (isTopIndex(i)) {
        try {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "\ttop" <<  (__project->FEMList().at(i.row())->getModes().empty() ? WithoutModes : WithModes);
#endif
            return __project->FEMList().at(i.row())->getModes().empty() ? WithoutModes : WithModes;
        } catch (...) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "\ttopReject";
#endif
            return 0;
        }
    }
    if (!isInfoIndex(i)) {
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "\tinModes" << 0;
#endif
        return 0;
    }
    switch (modelRole(i)) {
    case Modes:
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "\tmodes";
#endif
        try {
            const EigenModes& modes(__project->FEMList().at(modelId(i))->getModes());
            return static_cast<int>(modes.size());
        } catch (...) {
            return 0;
        }

    default:
#ifdef VIEWERMODEL_DEBUG
            qDebug() << "\tenother";
#endif
        return 0;
    }
}

QVariant FEMModel::headerData(int, Qt::Orientation, int role) const {
    if (role == Qt::DisplayRole) {
        return Application::identity()->tr("models list", "viewer model");
    }
    return QVariant();
}

bool FEMModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    assert(role == Qt::EditRole);
    if (role != Qt::EditRole) {
        return false;
    }
    try {
        Project::ConstModels models(__project->FEMList());
        Application::nonConstProject()->constCast(models.at(index.row()))->setName(value.toString());
        emit dataChanged(index, index, QVector<int>{ Qt::DisplayRole });
        return true;
    } catch(...) {
        return false;
    }
}

bool FEMModel::removeFEM(int id)
{
    try {
        this->beginRemoveRows(QModelIndex(), id, id);
        Application::nonConstProject()->deleteFEM(Application::project()->toFEM(id));
        this->endRemoveRows();
        return true;
    } catch(...) {
        return false;
    }
}

void FEMModel::addFEM(const QString& fname)
{
    FEM* fem(new FEM);
    try {
        try {
            fem->read(fname);
        } catch (std::exception e) {
            QMessageBox::warning(Application::mainWindow(),
                                 Application::identity()->tr("modes identification wizard/file warning/title"),
                                 QString(e.what()));
            return;
        }
    } catch(...) {
        return;
    }

    beginInsertRows(QModelIndex(), Application::project()->FEMCount(),
                                   Application::project()->FEMCount());
    Application::nonConstProject()->pushFEM(fem);
    endInsertRows();
}

void FEMModel::beginAddModes(const FEM* model)
{
    beginAddModes(FEMIndex(model));
}

void FEMModel::endAddModes(const FEM* model)
{
    endAddModes(FEMIndex(model));
}

void FEMModel::beginAddModes(const QModelIndex & i)
{
    if (WithoutModes < WithModes) {
        beginInsertRows(i, WithoutModes, WithModes - 1);
    } else {
        beginRemoveRows(i, WithModes, WithoutModes - 1);
    }
}

void FEMModel::endAddModes(const QModelIndex& i)
{
    if (WithoutModes < WithModes) {
        endInsertRows();
    } else {
        endRemoveRows();
    }
    const QModelIndex index(this->index(2, 0, i));
    emit this->dataChanged(index, index, QVector<int>{ Qt::DisplayRole });
}

EigenModesFounder* FEMModel::provideFounder(int modelId)
{
    FEM* const model(Application::nonConstProject()->FEMList().at(modelId));
    EigenModesFounder* const p(new EigenModesFounder(model, this));
    connect(p, &EigenModesFounder::finished, p, &EigenModesFounder::deleteLater);
    connect(p, &EigenModesFounder::modesMined, [this, &model](){ beginAddModes(FEMIndex(model)); });
    connect(p, &EigenModesFounder::modesReceived, [this, p]() {
        this->endAddModes(FEMIndex(p->model()));
    });
    connect(p, &EigenModesFounder::MACUpdated, [this, p]() {
        const QModelIndex index(this->index(1, 0, FEMIndex(p->model())));
        emit this->dataChanged(index, index, QVector<int>{ Qt::DisplayRole });
    });
    return p;
}

FEMModel::SharedEigenModesFounder FEMModel::foundModes(const int modelId, std::function<void(EigenModesFounder * const)> f)
{
    EigenModesFounder* const p(provideFounder(modelId));
    f(p);

    //trick to avoid delete(p) while thread is still running
    SharedEigenModesFounder* r(new SharedEigenModesFounder(p));
    connect(p, &EigenModesFounder::finished, [r]{ delete r; });

    return *r;
}

FEMModel::SharedEigenModesFounder FEMModel::importModes(const int modelId, const QString& file)
{
    return foundModes(modelId, [&file](EigenModesFounder* f){ f->readModes(file); });
}

FEMModel::SharedEigenModesFounder FEMModel::calculateModes(const int modelId, const IgoFESolver::SolverOptions& options)
{
    return foundModes(modelId, [&options](EigenModesFounder* f){ f->calculateModes(options); });
}

FEMModel::SharedEigenModesFounder FEMModel::identificateModes(const int modelId)
{
    return foundModes(modelId, [](EigenModesFounder* f){ f->identificateModes(); });
}

QVariant FEMModel::data(const QModelIndex & index, int role) const {
    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        if (role == Qt::DecorationRole && isTopIndex(index) && __project->FEMList().size() == index.row()) {
            static const QIcon add(Identity::fromSvg(":/media/resource/images/list-add-512px.svg"));
            return add;
        }
        return QVariant();
    }
    if (isRootIndex(index)) {
        return QVariant();
    }
#ifdef VIEWERMODEL_DEBUG
    qDebug() << "data" << index;
#endif
    if (isTopIndex(index)) {
        try {
            return __project->FEMList().at(index.row())->getName();
        } catch(...) {
            return Application::identity()->geometriesModelAdd();
        }
    }
    try {
        const FEM* const mesh(__project->FEMList().at(modelId(index)));
        if (isInfoIndex(index)) {
            ModelRow r(modelRole(index));
            static const QString vertexesStr(Application::identity()->tr("vertexes", "viewer model"));
            static const QString modesStr(Application::identity()->tr("modes", "viewer model"));
            static const QString importStr(Application::identity()->tr("import", "viewer model"));
            static const QString identificationStr(Application::identity()->tr("identification", "viewer model"));
            static const QString compute(Application::identity()->tr("compute", "viewer model"));
            static const QString macStr(Application::identity()->tr("mac", "viewer model"));
            static const QString macCalcStr(Application::identity()->tr("mac calc", "viewer model"));
            switch (r) {
            case FEMModel::Vertexes:
                return vertexesStr.arg(mesh->getNodes().length());
            case FEMModel::Modes:
                return modesStr.arg(mesh->getModes().size());
            case FEMModel::ImportModes:
                return importStr;
            case FEMModel::ModesIdentification:
                return identificationStr;
            case FEMModel::ModesCompute:
                return compute;
            case FEMModel::MAC:
                const CMatrix& mac(mesh->getModes().getMAC());
                if (mac.size() != mac.finiteCount()) {
                    return macCalcStr.arg(QString::number(mac.finiteCount()), QString::number(mac.size()));
                }
                return macStr;
            }
            return "something wrong";
        }
        return Application::identity()->formSelectorLabel().arg(QString::number(index.row() + 1), QString::number(mesh->getModes().at(index.row()).frequency()));
    } catch (...) {
        qFatal((QString("model id fail 0x") + QString::number(index.internalId(), 16) + ' ' + QString::number(modelId(index))).toLocal8Bit());
        return "Model id fail" + QString::number(modelId(index));
    }
}

void FEMModel::setProject(const Project* p) {
    __project = p;
}

const Project* FEMModel::getProject() {
    return __project;
}
