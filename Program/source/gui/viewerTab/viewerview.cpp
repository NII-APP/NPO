#include "viewerview.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPushButton>

#include "application.h"
#include "identity.h"
#include "project/femmodel.h"
#include "project.h"
#include "kernel/eigenmodesfounder.h"
#include "gui/modesIdentificationWizard/modesidentificationwizard.h"
#include "eigenmodes.h"
#include "fem.h"
#include "gui/mainWindow/mainwindow.h"
#include "kernel/igofesolver.h"

ViewerView::ViewerView(QWidget *parent)
    : QTreeView(parent)
{
    this->setModel(new FEMModel(Application::project(), this));
    connect(this->model(), &QAbstractItemModel::dataChanged, [this](const QModelIndex& a) { this->QAbstractItemView::update(a); });
    this->setFrameStyle(QFrame::NoFrame);
}

void ViewerView::currentChanged(const QModelIndex & current, const QModelIndex & prew) {
    const int modelId(FEMModel::modelId(current));
    if (modelId != FEMModel::modelId(prew) && modelId < myModel()->getProject()->FEMList().size()) {
        emit currentModelChanged(modelId);
    }
    if (current.internalId() && !FEMModel::isInfoIndex(current)) {
        emit currentModeChanged(current.row(), modelId);
    }
}
void ViewerView::mousePressEvent(QMouseEvent* e) {
    const QModelIndex current(this->indexAt(e->pos()));
    const int modelId(FEMModel::modelId(current));
    QTreeView::mousePressEvent(e);
    if (this->model()->data(current) == Application::identity()->geometriesModelAdd()) {
        addFEM();
    }

    const FEMModel::ModelRow r(static_cast<FEMModel*>(model())->modelRole(current));
    if (r == FEMModel::ImportModes) {
        importModes(modelId);
    } else if (r == FEMModel::ModesIdentification) {
        myModel()->identificateModes(modelId);
    } else if (r == FEMModel::MAC) {
        emit MACPressed(modelId, buf.contains(modelId) ? buf[modelId] : FEMModel::SharedEigenModesFounder());
    } else if (r == FEMModel::ModesCompute) {
        FEMModel::SharedEigenModesFounder proc(myModel()->calculateModes(modelId, IgoFESolver::SolverOptions()));
        connect(proc.get(), &EigenModesFounder::finished, [this, modelId](){
            buf.remove(modelId);
        });
        buf.insert(modelId, proc);
    }
}
void ViewerView::importModes(int modelId)
{
    QString file(Application::identity()->choseModesFile());
    if (!QFile::exists(file)) {
        return;
    }
    FEMModel::SharedEigenModesFounder proc(myModel()->importModes(modelId, file));
    connect(&*proc, &EigenModesFounder::finished, [this, modelId](){
        buf.remove(modelId);
    });
    buf.insert(modelId, proc);
}

void ViewerView::addFEM()
{
    QString bdf(Application::identity()->choseModelFile());
    if (!QFile::exists(bdf)) {
        return;
    }

    myModel()->addFEM(bdf);
    emit currentModelChanged(Application::project()->FEMCount() - 1);
    setCurrentIndex(myModel()->index(Application::project()->FEMCount() - 1, 0, rootIndex()));
}

FEMModel* ViewerView::myModel() const
{
    return static_cast<FEMModel*>(model());
}

void ViewerView::acceptNewProject() {
    myModel()->setProject(Application::project());
    this->reset();
}

void ViewerView::update() {
    QTreeView::reset();
}
