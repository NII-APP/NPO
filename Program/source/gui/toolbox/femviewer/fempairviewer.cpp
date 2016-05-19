#include "fempairviewer.h"

#include "femwidget.h"
#include "fempair.h"

FEMPairViewer::FEMPairViewer(QWidget* parent)
    : QWidget(parent)
    , screen(new FEMWidget(this))
{
    this->setMinimumWidth(100);
}

QSize FEMPairViewer::minimumSizeHint()
{
    return QSize(500,500);
}

void FEMPairViewer::setPair(const FEMPair* p)
{
    qDebug() << "set FEM pair";
    if (p == nullptr) {
        screen->setData(nullptr);
        return;
    }
    screen->setData(QList<const FEM*>() << p->underUpdate() << p->updater() << p->truncated());
}

void FEMPairViewer::setMode(const int v)
{
    screen->setMode(v);
}

void FEMPairViewer::resizeEvent(QResizeEvent *)
{
    screen->move(QPoint(0,0));
    screen->resize(this->size());
}
