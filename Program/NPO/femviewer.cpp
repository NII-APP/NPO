#include "femviewer.h"

FEMViewer::FEMViewer(QWidget* parent)
    : FEMWidget(parent)
    , toolbox(new QToolBox(this))
{
    toolbox->move(0,0);
    toolbox->setAutoFillBackground(true);
    toolbox->addAction(new QAction("hello", toolbox));
    toolbox->setVisible(false);
    this->setMouseTracking(true);
    QPalette p(this->palette());
    p.setColor(QPalette::Background, QColor(0xFFFF00FF));
    this->setPalette(p);
}

void FEMViewer::mouseMoveEvent(QMouseEvent * e) {
    toolbox->setVisible(toolbox->geometry().contains(e->pos()));
}

void FEMViewer::leaveEvent(QEvent *) {
    toolbox->setVisible(false);
}

void FEMViewer::resizeCGL(int w, int) {
    toolbox->resize(w, toolbox->height());
}
