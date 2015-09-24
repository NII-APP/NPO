#include "pairsview.h"
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

void PairsView::setProject(const Project* p) {
    myModel()->setProject(p);
    this->update(QModelIndex());
}
