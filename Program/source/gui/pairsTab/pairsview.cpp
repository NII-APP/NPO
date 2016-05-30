#include "pairsview.h"

#include <QMouseEvent>
#include <QDebug>

#include "pairsmodel.h"
#include "project.h"
#include "application.h"
#include "pairsmodel.h"

PairsView::PairsView(QWidget *parent)
    : QTreeView(parent)
    , currentPair(-1)
{
    this->setModel(new PairModel(this));
    myModel()->setProject(Application::project());
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

    const QModelIndex here(this->indexAt(event->pos()));
    if (model()->data(here, Qt::DisplayRole) == Application::identity()->tr("add item", "pair tab/model")) {
        emit addPairPressed();
    }
}


void PairsView::currentChanged(const QModelIndex & current, const QModelIndex & prew) {
    const int pairId(PairModel::pairId(current));
    if (pairId != PairModel::pairId(prew) && pairId < myModel()->getProject()->pairsList().size()) {
        emit currentPairChanged(pairId);
    }
}

PairModel* PairsView::model() const
{
    return static_cast<PairModel*>(QTreeView::model());
}
