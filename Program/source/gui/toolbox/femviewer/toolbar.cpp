#include "toolbar.h"

#include <QLayout>

#include "modeinput.h"
#include "frequencyinput.h"
#include "magnitudeinput.h"
#include "identity.h"
#include "application.h"
#include "femwidget.h"

ToolBar::ToolBar(FEMWidget* femWidget, QWidget *parent)
    : QToolBar(parent)
    , mode(new ModeInput(femWidget, this))
    , frequency(new FrequencyInput(this))
    , magnitude(new MagnitudeInput(this))
    , pauseIcon(Identity::fromSvg(":/media/resource/images/pause-512px.svg"))
    , playIcon(Identity::fromSvg(":/media/resource/images/play-512px.svg"))
    , run(new QAction(pauseIcon, Application::identity()->tr("pause", "FEMViewer"),this))
    , stop(new QAction(Identity::fromSvg(":/media/resource/images/stop-512px.svg"), Application::identity()->tr("stop", "FEMViewer"),this))
    , femWidget(femWidget)
{
    this->setAutoFillBackground(true);
    this->setVisible(false);
    this->layout()->setMargin(0);

    magnitude->setValue(femWidget->getAnimationOptions()->getMagnitude());
    connect(magnitude, SIGNAL(valueChanged(double)), femWidget, SLOT(setMagnitude(double)));
    connect(femWidget, SIGNAL(magnitudeChanged(double)), magnitude, SLOT(setValue(double)));
    this->addWidget(magnitude);

    this->addSeparator();

    frequency->setValue(femWidget->getAnimationOptions()->getFrequency());
    connect(frequency, SIGNAL(valueChanged(double)), femWidget, SLOT(setFrequency(double)));
    connect(frequency, &FrequencyInput::valueChanged, [femWidget, this](){
        if (femWidget->getAnimationOptions()->isPaused()) {
            femWidget->getAnimationOptions()->play();
            run->setIcon(pauseIcon);
        }
    });
    connect(femWidget, SIGNAL(frequencyChanged(double)), frequency, SLOT(setValue(double)));
    this->addWidget(frequency);

    this->addSeparator();

    connect(mode, SIGNAL(valueChanged(int)), femWidget, SLOT(setMode(int)));
    connect(mode, &ModeInput::valueChanged, [femWidget](int i) { femWidget->colorize(i); });
    this->addWidget(mode);

    this->addSeparator();

    stop->setCheckable(true);
    run->setDisabled(true);
    stop->setDisabled(true);
    this->addActions(QList<QAction*>() << run << stop);
    this->connect(run, &QAction::triggered, [femWidget, this](){
        if (femWidget->getAnimationOptions()->isPaused()) {
            femWidget->getAnimationOptions()->play();
            run->setText(Application::identity()->tr("play", "FEMViewer"));
            run->setIcon(playIcon);
        } else {
            femWidget->getAnimationOptions()->pause();
            run->setText(Application::identity()->tr("pause", "FEMViewer"));
            run->setIcon(pauseIcon);
        }
    });
    this->connect(stop, &QAction::toggled, [femWidget, this](bool v) {
        run->setDisabled(v);
        femWidget->getAnimationOptions()->setMagnitude(magnitude->getValue() * !v);
    });
}

void ToolBar::refresh()
{
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

void ToolBar::setMode(const int m)
{
    refresh();
    mode->setValue(m);
}

void ToolBar::setProxyModeState()
{
    mode->hide();
    frequency->setEnabled(true);
    magnitude->setEnabled(true);
    run->setEnabled(true);
    stop->setEnabled(true);
}

ToolBar::~ToolBar()
{

}

