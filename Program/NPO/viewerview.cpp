#include "viewerview.h"
#include "application.h"
#include "identity.h"

ViewerView::ViewerView(QWidget *parent)
    : QTreeView(parent)
{

}

void ViewerView::currentChanged(const QModelIndex & current, const QModelIndex &) {
    if (this->model()->data(current) == Application::identity()->geometriesModelAdd()) {
        emit addModelPressed();
    }
}
