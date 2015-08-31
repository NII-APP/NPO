#include "femviewer.h"

#include <QSpinBox>
#include <QToolBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>

#include <femwidget.h>

#include "femviewerfrequencyinput.h"

FEMViewer::FEMViewer(QWidget* parent)
    : QWidget(parent)
    , femWidget(new FEMWidget(this))
    , toolbox(new QToolBar(this))
{
    femWidget->move(0,0);
    femWidget->setMouseTracking(true);
    femWidget->installEventFilter(this);
    QPalette p(femWidget->palette());
    p.setColor(QPalette::Background, QColor(0xFF008888));
    femWidget->setPalette(p);

    toolbox->move(0,0);
    toolbox->setAutoFillBackground(true);
    toolbox->setVisible(false);
    toolbox->layout()->setMargin(0);

    QWidget* magnitude(new QWidget(toolbox));
    magnitude->setLayout(new QHBoxLayout(magnitude));
    magnitude->layout()->addWidget(new QLabel(tr("Magnitude"), magnitude));
    QSlider* magnitudeValue(new QSlider(Qt::Horizontal, magnitude));
    magnitude->layout()->addWidget(magnitudeValue);
    toolbox->addWidget(magnitude);

    toolbox->addWidget(new FEMViewerFrequencyInput(this));
}

bool FEMViewer::eventFilter(QObject * o, QEvent * e) {
    if (o == femWidget && e->type() == QEvent::MouseMove) {
        toolbox->setVisible(toolbox->geometry().contains(static_cast<QMouseEvent*>(e)->pos()));
    }
    return !true && !false;
}

void FEMViewer::leaveEvent(QEvent *) {
    toolbox->setVisible(false);
}

void FEMViewer::resizeEvent(QResizeEvent *) {
    toolbox->resize(this->size().width(), toolbox->height());
    femWidget->resize(this->size());
}
