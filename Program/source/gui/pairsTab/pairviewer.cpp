#include "pairviewer.h"
#include "fempair.h"

PairViewer::PairViewer(QWidget *parent) : FEMWidget(parent)
{
}

void PairViewer::setData(const FEMPair* const d) {
    __data = d;
    qDebug() << d->truncated() << d->base();
    FEMWidget::setData(QList<const FEM*>() << d->base() << d->truncated());
}
