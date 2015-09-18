#include "viewernode.h"

#include <c3dcolumnchart.h>
#include <fem.h>

#include "femviewer.h"

ViewerNode::ViewerNode(QWidget *parent)
    : QSplitter(parent)
    , __viewer(new FEMViewer(this))
#ifndef MAC_DISABLE
    , __macChart(new CGL::C3dColumnChart(this))
#endif
{
    __viewer->setMinimumWidth(100);
#ifndef MAC_DISABLE
    __macChart->setMinimumWidth(100);
#endif
    this->addWidget(__viewer);

    this->setHandleWidth(1);

#ifndef MAC_DISABLE
    this->addWidget(__macChart);
    __macChart->hide();
    this->setSizes(QList<int>() << 200 << 100);
#endif
}

void ViewerNode::setModel(const FEM* const model) {
    __viewer->setModel(model);
    update();
}

void ViewerNode::setMode(int m) {
    __viewer->colorize(m);
    __viewer->setMode(m);
}

void ViewerNode::update() {
    __viewer->updateToolBar();
#ifndef MAC_DISABLE
    if (__viewer->getModel()->getModes().empty()) {
        __macChart->hide();
    } else {
        __macChart->show();
        __macChart->setData(__viewer->getModel()->getModes().getMac());
    }
#endif
}
