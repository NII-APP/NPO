#include "femscreen.h"

#include <QSpinBox>
#include <QToolBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QSvgRenderer>
#include <QAction>

#include <femwidget.h>
#include <fem.h>

#include "frequencyinput.h"
#include "modeinput.h"
#include "magnitudeinput.h"
#include "identity.h"
#include "application.h"
#include "identity.h"
#include "toolbar.h"

FEMScreen::FEMScreen(QWidget* parent)
    : QWidget(parent)
    , femWidget(new FEMWidget(this))
    , toolbox(new ToolBar(femWidget, this))
{
    femWidget->setVisible(false);
    femWidget->move(0,0);
    femWidget->setMouseTracking(true);
    femWidget->installEventFilter(this);

    toolbox->move(0,0);
    toolbox->adjustSize();
    toolbox->setVisible(false);
}

FEMScreen::~FEMScreen()
{
}

QSize FEMScreen::sizeHint() const
{
    return QSize(500,500);
}

QSize FEMScreen::minimumSizeHint()
{
    return QSize(500,500);
}

bool FEMScreen::eventFilter(QObject * o, QEvent * e)
{
    if (o == femWidget && e->type() == QEvent::MouseMove) {
        toolbox->setVisible(toolbox->geometry().contains(static_cast<QMouseEvent*>(e)->pos()) || toolbox->isLocked());
    }
    return !true && !false;
}

void FEMScreen::leaveEvent(QEvent *)
{
    toolbox->setVisible(toolbox->isLocked());
}

void FEMScreen::resizeEvent(QResizeEvent *)
{
    toolbox->resize(this->size().width(), toolbox->height());
    femWidget->move(0,0);
    femWidget->resize(this->size());
}

void FEMScreen::paintEvent(QPaintEvent *)
{
    if (femWidget->isVisible()) {
        return;
    }
    static const QPixmap bg(":/media/resource/images/hill.png");
    QPainter painter(this);
    painter.drawPixmap(QRect(QPoint(this->width() / 2.0 - bg.width() / 2.0, this->height() / 2.0 - bg.height() / 2.0), bg.size()), bg);
}

void FEMScreen::setMode(const int m)
{
    toolbox->setMode(m);
}

void FEMScreen::updateToolBar()
{
    toolbox->refresh();
}
