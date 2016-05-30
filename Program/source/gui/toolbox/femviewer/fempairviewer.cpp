#include "fempairviewer.h"

#include <cassert>

#include <QMouseEvent>
#include <QLabel>
#include <QResizeEvent>
#include <QWidgetAction>
#ifdef Q_OS_WIN
#include <QtWin>
#endif

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
    , hi(([this]()->QAction*{ toolbox->addSeparator(); return this->toolbox->addAction(Application::identity()->icon("FEMPairViewer/hi icon"), Application::identity()->tr("FEMPairViewer/hi")); })())
    , low(this->toolbox->addAction(Application::identity()->icon("FEMPairViewer/low icon"), Application::identity()->tr("FEMPairViewer/low")))
    , trunc(this->toolbox->addAction(Application::identity()->icon("FEMPairViewer/trunc icon"), Application::identity()->tr("FEMPairViewer/trunc")))
    , info(new QLabel(this, Qt::SubWindow))
{

    info->setAutoFillBackground(true);
    info->setMargin(10);
    info->hide();
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
    pair = p;
    femWidget->setVisible(p);
    if (p == nullptr) {
        femWidget->setData(nullptr);
        updateToolBar();
        return;
    }

    femWidget->setData(QList<const FEM*>() << p->practic() << p->theory() << p->truncated());
    updateToolBar();
    mode->setMaximum(static_cast<int>(p->a()->getModes().size()), static_cast<int>(p->b()->getModes().size()));
    mode->updateRelations(p->relations());
}

void FEMPairViewer::updateRelations(const CIndexes& r)
{
    mode->updateRelations(r);
}

void FEMPairViewer::resizeEvent(QResizeEvent* e)
{
    FEMScreen::resizeEvent(e);
    info->move((QPoint(0, this->height() - info->height())));
}

void FEMPairViewer::moveEvent(QMoveEvent* e)
{
    FEMScreen::moveEvent(e);
    info->move(this->mapToGlobal(QPoint(0, toolbox->height())));
}

void FEMPairViewer::setMode(int l, int r)
{
    if (!pair) {
        return;
    }
    info->show();
    femWidget->setMode(l, pair->a());
    femWidget->colorize(l, "", pair->a());
    femWidget->setMode(r, pair->b());
    femWidget->colorize(r, "", pair->b());
    femWidget->setMode(pair->theory() == pair->a() ? l : r, pair->truncated());
    femWidget->colorize(pair->theory() == pair->a() ? l : r, "", pair->truncated());
    info->setText(Application::identity()->tr("info", "FEMPairViewer").arg(QString::number(l + 1), QString::number(pair->a()->getModes().at(l).frequency()),
                                                                           QString::number(r + 1), QString::number(pair->b()->getModes().at(r).frequency())));
    info->resize(info->sizeHint());
}
