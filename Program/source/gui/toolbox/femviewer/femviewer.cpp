#include "femviewer.h"

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

FEMViewer::FEMViewer(QWidget* parent)
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
}

FEMViewer::~FEMViewer()
{
}

QSize FEMViewer::sizeHint() const
{
    return QSize(500,500);
}

bool FEMViewer::eventFilter(QObject * o, QEvent * e)
{
    if (o == femWidget && e->type() == QEvent::MouseMove) {
        toolbox->setVisible(toolbox->geometry().contains(static_cast<QMouseEvent*>(e)->pos()));
    }
    return !true && !false;
}

void FEMViewer::leaveEvent(QEvent *)
{
    toolbox->setVisible(false);
}

void FEMViewer::resizeEvent(QResizeEvent *)
{
    toolbox->resize(this->size().width(), toolbox->height());
    femWidget->move(0,0);
    femWidget->resize(this->size());
}

void FEMViewer::paintEvent(QPaintEvent *)
{
    if (femWidget->isVisible()) {
        return;
    }
    static const QPixmap bg(":/media/resource/images/hill.png");
    QPainter painter(this);
    painter.drawPixmap(QRect(QPoint(this->width() / 2.0 - bg.width() / 2.0, this->height() / 2.0 - bg.height() / 2.0), bg.size()), bg);
}

void FEMViewer::setModel(const FEM* m) const
{
    femWidget->setVisible(m);
    femWidget->setData(m);
    updateToolBar();

    femWidget->resize(this->size());
}

void FEMViewer::setMode(const int m)
{
    toolbox->setMode(m);
}

void FEMViewer::updateToolBar() const
{
    toolbox->refresh();
}

void FEMViewer::colorize(int m)
{
    femWidget->colorize(m);
}

const FEM* FEMViewer::getModel() const
{
    if (!femWidget->getData().isEmpty()) {
        return femWidget->getData().first();
    } else {
        return nullptr;
    }
}

void FEMViewer::setProxyMode(const EigenMode& m)
{
    femWidget->setProxyMode(m);
    toolbox->setProxyModeState();
}
