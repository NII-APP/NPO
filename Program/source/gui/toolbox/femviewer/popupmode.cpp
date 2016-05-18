#include "popupmode.h"

#include <QGridLayout>
#include <QSizeGrip>
#include <QDebug>

#include "femviewer.h"

PopupMode::PopupMode(QWidget *parent)
    : QFrame(parent, Qt::Popup)
    , screen(new FEMViewer(this))
    , grip(new QSizeGrip(this))
{
    setContentsMargins(0,0,0,0);
    setFrameStyle(QFrame::Plain | QFrame::StyledPanel);
    setLineWidth(1);

    this->setLayout(new QGridLayout);
    layout()->addWidget(screen, 0,0);
    layout()->addWidget(grip, 1,1);

    grip->setCursor(Qt::SizeFDiagCursor);
}

QGridLayout* PopupMode::layout() const
{
    return static_cast<QGridLayout*>(QFrame::layout());
}

PopupMode::~PopupMode() { }

void PopupMode::resizeEvent(QResizeEvent *)
{
    screen->move(0,0);
    screen->resize(this->size());
    grip->move(this->width() - grip->width(), this->height() - grip->height());
}

void PopupMode::setModel(const FEM* const v)
{
    screen->setModel(v);
}

void PopupMode::setMode(int v)
{
    screen->setMode(v);
}

void PopupMode::closeEvent(QCloseEvent *)
{
    emit closed();
}
