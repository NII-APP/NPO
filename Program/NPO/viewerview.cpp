#include "viewerview.h"
#include "application.h"
#include "identity.h"
#include "viewermodel.h"
#include <QDebug>

ViewerView::ViewerView(QWidget *parent)
    : QTreeView(parent)
{
}

void ViewerView::currentChanged(const QModelIndex & current, const QModelIndex & prew) {
    if (this->model()->data(current) == Application::identity()->geometriesModelAdd()) {
        emit addModelPressed();
    }
    auto goTop = [](const QModelIndex& i, const QAbstractItemModel* const m)->QModelIndex {
        QModelIndex r(i);
        while (!ViewerModel::isTopIndex(r) && !ViewerModel::isRootIndex(r)) {
            r = m->parent(r);
        }
        return r;
    };
    const int modelId(ViewerModel::modelId(current));
    if (modelId != ViewerModel::modelId(prew)) {
        emit currentModelChanged(modelId);
    }
    const ViewerModel::ModelRow r(static_cast<ViewerModel*>(model())->modelRole(current));
    if (r == ViewerModel::ImportModes) {
        emit importModesPressed(modelId);
    } else if (r == ViewerModel::ModesIdentification) {
        emit modesIdentificationPressed(modelId);
    }
    if (current.internalId() && !ViewerModel::isInfoIndex(current)) {
        emit currentModeChanged(current.row(), modelId);
    }
}
