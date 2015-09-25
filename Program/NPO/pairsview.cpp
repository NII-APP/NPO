#include "pairsview.h"

#include <QMouseEvent>
#include <QDebug>

#include "pairsmodel.h"
#include "project.h"
#include "application.h"

PairsView::PairsView(QWidget *parent) : QTreeView(parent)
{
    this->setModel(new PairModel(this));
    this->setFrameStyle(QFrame::NoFrame);
}

PairModel* PairsView::myModel() const {
    return static_cast<PairModel*>(model());
}

void PairsView::acceptNewProject() {
    myModel()->setProject(Application::project());
    this->update(QModelIndex());
}

void PairsView::mousePressEvent(QMouseEvent *event) {
    QTreeView::mousePressEvent(event);
    if (model()->data(this->indexAt(event->pos())) == Application::identity()->tr("add item", "pair tab/model")) {
        emit addPairPressed();
    }
}
