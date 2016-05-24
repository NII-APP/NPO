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
#include "application.h"
#include "identity.h"

FEMPairViewer::FEMPairViewer(QWidget* parent)
    : FEMScreen(parent)
    , mode(new RelationModeInput(toolbox))
    , hi(this->toolbox->addAction(Application::identity()->icon("FEMPairViewer/hi icon"), Application::identity()->tr("FEMPairViewer/hi")))
    , low(this->toolbox->addAction(Application::identity()->icon("FEMPairViewer/low icon"), Application::identity()->tr("FEMPairViewer/low")))
    , trunc(this->toolbox->addAction(Application::identity()->icon("FEMPairViewer/trunc icon"), Application::identity()->tr("FEMPairViewer/trunc")))
{
    auto init = [this] (QAction* const act, int id) {
        act->setCheckable(true);
        act->setChecked(true);
        connect(act, &QAction::triggered, [id, this](bool v) {
            femWidget->setVisible(v, id);
        });
    };
    init(low, 0);
    init(hi, 1);
    init(trunc, 2);
    for (QAction* i : toolbox->actions()) if (dynamic_cast<QWidgetAction*>(i)) {
        if (dynamic_cast<QWidgetAction*>(i)->defaultWidget() == toolbox->modeInput()) {
            toolbox->modeInput()->hide();
            toolbox->insertWidget(i, mode);
            toolbox->removeAction(i);
        }
    }
    assert(toolbox->modeInput()->isHidden());
    connect(mode, &RelationModeInput::valueChanged, this, &FEMPairViewer::setMode);
}

void FEMPairViewer::setPair(const FEMPair* p)
{
    qDebug() << "set FEM pair";
    pair = p;
    femWidget->setVisible(p);
    if (p == nullptr) {
        femWidget->setData(nullptr);
        return;
    }

    femWidget->setData(QList<const FEM*>() << p->practic() << p->theory() << p->truncated());
    updateToolBar();
    mode->setMaximum(static_cast<int>(p->a()->getModes().size()), static_cast<int>(p->b()->getModes().size()));
    mode->updateRelations();
}

void FEMPairViewer::updateRelations(const CIndexes& r)
{
    mode->updateRelations(r);
}

void FEMPairViewer::setMode(int l, int r)
{
    if (!pair) {
        return;
    }
    femWidget->setMode(l, pair->a());
    femWidget->colorize(l, "", pair->a());
    femWidget->setMode(r, pair->b());
    femWidget->colorize(r, "", pair->b());
    femWidget->setMode(pair->theory() == pair->a() ? l : r, pair->truncated());
    femWidget->colorize(pair->theory() == pair->a() ? l : r, "", pair->truncated());
}
