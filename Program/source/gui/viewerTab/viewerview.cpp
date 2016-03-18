#include "viewerview.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPushButton>

#include "application.h"
#include "identity.h"
#include "viewermodel.h"
#include "project.h"
#include "femprocessor.h"

ViewerView::ViewerView(QWidget *parent)
    : QTreeView(parent)
{
    this->setModel(new ViewerModel(Application::project(), this));
    connect(this->model(), &QAbstractItemModel::dataChanged, [this](const QModelIndex& a) { this->QAbstractItemView::update(a); });
    this->setFrameStyle(QFrame::NoFrame);
}

void ViewerView::currentChanged(const QModelIndex & current, const QModelIndex & prew) {
    const int modelId(ViewerModel::modelId(current));
    if (modelId != ViewerModel::modelId(prew) && modelId < myModel()->getProject()->FEMList().size()) {
        emit currentModelChanged(modelId);
    }
    if (current.internalId() && !ViewerModel::isInfoIndex(current)) {
        emit currentModeChanged(current.row(), modelId);
    }
}
void ViewerView::mousePressEvent(QMouseEvent* e) {
    const QModelIndex current(this->indexAt(e->pos()));
    const int modelId(ViewerModel::modelId(current));
    QTreeView::mousePressEvent(e);
    if (this->model()->data(current) == Application::identity()->geometriesModelAdd()) {
        addFEM();
    }

    const ViewerModel::ModelRow r(static_cast<ViewerModel*>(model())->modelRole(current));
    if (r == ViewerModel::ImportModes) {
        importModes(modelId);
    } else if (r == ViewerModel::ModesIdentification) {
        emit modesIdentificationPressed(modelId);
    } else if (r == ViewerModel::MAC) {
        emit MACPressed(modelId, buf.contains(modelId) ? buf[modelId] : nullptr);
    } else if (r == ViewerModel::ModesCompute) {
        emit calcModes(modelId);
    }
}

void ViewerView::importModes(int meshId)
{
    QString file(Application::identity()->choseModesFile());
    if (!QFile::exists(file)) {
        return;
    }
    FEMProcessor* const proc(myModel()->importModes(meshId, file));
    buf.insert(meshId, proc);
    connect(proc, &FEMProcessor::finished, [meshId, this](){ this->buf.remove(meshId); });
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

ViewerModel* ViewerView::myModel() const
{
    return static_cast<ViewerModel*>(model());
}

void ViewerView::acceptNewProject() {
    myModel()->setProject(Application::project());
    this->reset();
}

void ViewerView::update() {
    QTreeView::reset();
}
