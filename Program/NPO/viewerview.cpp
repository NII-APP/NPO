#include "viewerview.h"

#include <QDebug>
#include <QMouseEvent>

#include "application.h"
#include "identity.h"
#include "viewermodel.h"
#include "project.h"

ViewerView::ViewerView(QWidget *parent)
    : QTreeView(parent)
{
    this->setModel(new ViewerModel(Application::project(), this));
    this->setFrameStyle(QFrame::NoFrame);
}

void ViewerView::currentChanged(const QModelIndex & current, const QModelIndex & prew) {
    const int modelId(ViewerModel::modelId(current));
    if (modelId != ViewerModel::modelId(prew) && modelId < myModel()->getProject()->modelsList().size()) {
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
        emit addModelPressed();
    }

    const ViewerModel::ModelRow r(static_cast<ViewerModel*>(model())->modelRole(current));
    if (r == ViewerModel::ImportModes) {
        emit importModesPressed(modelId);
    } else if (r == ViewerModel::ModesIdentification) {
        emit modesIdentificationPressed(modelId);
    } else if (r == ViewerModel::MAC) {
        emit MACPressed(modelId);
    }
}

void ViewerView::updateCurrentModel() {
    QTreeView::update(myModel()->index(ViewerModel::modelId(this->currentIndex()), 0, QModelIndex()));
}

ViewerModel* ViewerView::myModel() const { return static_cast<ViewerModel*>(model()); }
void ViewerView::acceptNewProject() {
    myModel()->setProject(Application::project());
    this->reset();
}

void ViewerView::update() {
    QTreeView::reset();
}

void ViewerView::updateModel(int id) {
    QTreeView::dataChanged(myModel()->index(id, 0, QModelIndex()),
                           myModel()->index(id, 0, QModelIndex()));
}
