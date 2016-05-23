#include "fempairviewer.h"

#include <cassert>

#include <QMouseEvent>
#include <QLabel>
#include <QResizeEvent>
#include <QWidgetAction>

#include "femwidget.h"
#include "fempair.h"
#include "toolbar.h"
#include "modeinput.h"
#include "relationmodeinput.h"

FEMPairViewer::FEMPairViewer(QWidget* parent)
    : FEMScreen(parent)
{
    //toolbox->modeInput()->hide();
    for (QAction* i : toolbox->actions()) if (dynamic_cast<QWidgetAction*>(i)) {
        if (dynamic_cast<QWidgetAction*>(i)->defaultWidget() == toolbox->modeInput()) {
            toolbox->modeInput()->hide();
            toolbox->insertWidget(i, new RelationModeInput(toolbox));
            toolbox->removeAction(i);
        }
    }
    assert(toolbox->modeInput()->isHidden());
}

void FEMPairViewer::setPair(const FEMPair* p)
{
    qDebug() << "set FEM pair";
    femWidget->setVisible(p);
    if (p == nullptr) {
        femWidget->setData(nullptr);
        return;
    }
    femWidget->setData(QList<const FEM*>() << p->underUpdate() << p->updater() << p->truncated());
    updateToolBar();
}

void FEMPairViewer::setMode(const int v)
{
    femWidget->setMode(v);
}
