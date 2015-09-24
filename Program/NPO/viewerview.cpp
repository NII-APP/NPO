#include "viewerview.h"
#include "application.h"
#include "identity.h"
#include "viewermodel.h"
#include <QDebug>

ViewerView::ViewerView(QWidget *parent)
    : QTreeView(parent)
{
    this->setModel(new ViewerModel(Application::project(), this));
    this->setFrameStyle(QFrame::NoFrame);
}

void ViewerView::currentChanged(const QModelIndex & current, const QModelIndex & prew) {
    if (this->model()->data(current) == Application::identity()->geometriesModelAdd()) {
        emit addModelPressed();
    }
    const int modelId(ViewerModel::modelId(current));
    if (modelId != ViewerModel::modelId(prew)) {
        emit currentModelChanged(modelId);
    }
    const ViewerModel::ModelRow r(static_cast<ViewerModel*>(model())->modelRole(current));
    if (r == ViewerModel::ImportModes) {
        emit importModesPressed(modelId);
    } else if (r == ViewerModel::ModesIdentification) {
        emit modesIdentificationPressed(modelId);
    } else if (r == ViewerModel::MAC) {
        emit MACPressed(modelId);
    }
    if (current.internalId() && !ViewerModel::isInfoIndex(current)) {
        emit currentModeChanged(current.row(), modelId);
    }
}

ViewerModel* ViewerView::myModel() const { return static_cast<ViewerModel*>(model()); }
void ViewerView::acceptNewProject() {
    myModel()->setProject(Application::project());
    this->reset();
}

void ViewerView::update() {
    this->reset();
}
