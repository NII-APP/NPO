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

#include "femviewerfrequencyinput.h"
#include "femviewermodeinput.h"
#include "femviewermagnitudeinput.h"
#include "identity.h"

FEMViewer::FEMViewer(QWidget* parent)
    : QWidget(parent)
    , femWidget(new FEMWidget(this))
    , toolbox(new QToolBar(this))
    , mode(new FEMViewerModeInput(this, toolbox))
    , frequency(new FEMViewerFrequencyInput(toolbox))
    , magnitude(new FEMViewerMagnitudeInput(toolbox))
    , pause(Identity::fromSvg(":/media/images/pause-512px.svg"))
    , play(Identity::fromSvg(":/media/images/play-512px.svg"))
    , run(new QAction(pause, tr("pause"),toolbox))
    , stop(new QAction(Identity::fromSvg(":/media/images/stop-512px.svg"), tr("stop"),toolbox))
{
    femWidget->setVisible(false);
    femWidget->move(0,0);
    femWidget->setMouseTracking(true);
    femWidget->installEventFilter(this);

    toolbox->move(0,0);
    toolbox->setAutoFillBackground(true);
    toolbox->setVisible(false);
    toolbox->layout()->setMargin(0);

    magnitude->setValue(femWidget->getAnimationOptions()->getMagnitude());
    connect(magnitude, SIGNAL(valueChanged(double)), femWidget, SLOT(setMagnitude(double)));
    connect(femWidget, SIGNAL(magnitudeChanged(double)), magnitude, SLOT(setValue(double)));
    toolbox->addWidget(magnitude);

    toolbox->addSeparator();

    frequency->setValue(femWidget->getAnimationOptions()->getFrequency());
    connect(frequency, SIGNAL(valueChanged(double)), femWidget, SLOT(setFrequency(double)));
    connect(frequency, SIGNAL(valueChanged(double)), this, SLOT(runEnable()));
    connect(femWidget, SIGNAL(frequencyChanged(double)), frequency, SLOT(setValue(double)));
    toolbox->addWidget(frequency);

    toolbox->addSeparator();

    connect(mode, SIGNAL(valueChanged(int)), femWidget, SLOT(setMode(int)));
    toolbox->addWidget(mode);

    toolbox->addSeparator();

    stop->setCheckable(true);
    run->setDisabled(true);
    stop->setDisabled(true);
    toolbox->addActions(QList<QAction*>() << run << stop);
    this->connect(run, SIGNAL(triggered(bool)), SLOT(runTrigger()));
    this->connect(stop, SIGNAL(toggled(bool)), SLOT(stopTrigger(bool)));

    toolbox->adjustSize();
}

void FEMViewer::runTrigger() {
    if (femWidget->getAnimationOptions()->isPaused()) {
        femWidget->getAnimationOptions()->play();
        run->setText(tr("run"));
        run->setIcon(pause);
    } else {
        femWidget->getAnimationOptions()->pause();
        run->setText(tr("pause"));
        run->setIcon(play);
    }
}
void FEMViewer::runEnable() {
    if (femWidget->getAnimationOptions()->isPaused()) {
        femWidget->getAnimationOptions()->play();
        run->setIcon(pause);
    }
}

void FEMViewer::stopTrigger(bool v) {
    run->setDisabled(v);
    femWidget->getAnimationOptions()->setMagnitude(magnitude->getValue() * !v);
}

QSize FEMViewer::sizeHint() const {
    return QSize(200,200);
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

void FEMViewer::paintEvent(QPaintEvent *) {
    if (femWidget->isVisible()) {
        return;
    }
    static const QPixmap bg(":/media/images/hill.png");
    QPainter painter(this);
    painter.drawPixmap(QRect(QPoint(this->width() / 2.0 - bg.width() / 2.0, this->height() / 2.0 - bg.height() / 2.0), bg.size()), bg);
}

void FEMViewer::setModel(const FEM* m) const {
    femWidget->setVisible(m);
    femWidget->setData(m);
    updateToolBar();

    femWidget->resize(this->size());
}

void FEMViewer::setMode(const int m) {
    femWidget->setMode(m);
}

void FEMViewer::updateToolBar() const {
    mode->updateValueBounds();
    bool isHaveModes(false);
    for (const FEM* i : femWidget->getData()) {
        isHaveModes = isHaveModes || (i && !i->getModes().empty());
    }
    frequency->setEnabled(isHaveModes);
    magnitude->setEnabled(isHaveModes);
    run->setEnabled(isHaveModes);
    stop->setEnabled(isHaveModes);
}

void FEMViewer::colorize(int m) {
    femWidget->colorize(m);
}

const FEM* FEMViewer::getModel() const {
    if (!femWidget->getData().isEmpty()) {
        return femWidget->getData().first();
    } else {
        return nullptr;
    }
}
