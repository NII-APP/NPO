#include "pairviewer.h"
#include "fempair.h"
#include "femwidget.h"

PairViewer::PairViewer(QWidget *parent)
    : QWidget(parent)
    , screen(new FEMWidget(this))
{
}

void PairViewer::setModel(const FEMPair* const d) {
    __data = d;
    //qDebug() << d->truncated() << d->base();
    screen->setData(d->truncated());
}

void PairViewer::setMode(int)
{

}
