#include "femviewerfrequencyinput.h"
#include <QDoubleSpinBox>
#include <QSlider>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QPicture>

#include "application.h"
#include "identity.h"

const FEMViewer::FEMViewerFrequencyInput::ToRealScale FEMViewer::FEMViewerFrequencyInput::SLIDER_SCALE =
        FEMViewer::FEMViewerFrequencyInput::ToRealScale(FEMViewer::FEMViewerFrequencyInput::ToRealScale::InRange(0, 10000),
                                             FEMViewer::FEMViewerFrequencyInput::ToRealScale::OutRange(0.2, 5.0));

FEMViewer::FEMViewerFrequencyInput::FEMViewerFrequencyInput(QWidget* parent)
    : QFrame(parent)
    , numeric(new QDoubleSpinBox(this))
    , slider(new QSlider(Qt::Horizontal, this)) {
    this->setLayout(new QHBoxLayout(this));
    this->layout()->addWidget(([this]()->QWidget* {
                                   QLabel* const l(new QLabel(this));
                                   static const QPixmap icon(([this]()->QPixmap{
                                       QPixmap m(":/media/resource/images/freq.png");
                                       return m.scaled(this->height(), this->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                                   })());
                                   l->setToolTip(Application::identity()->tr("frequency", "FEMViewer"));
                                   l->setPixmap(icon);
                                   return l;
    })());
    this->setMinimumWidth(200);
    this->layout()->addWidget(slider);
    this->layout()->addWidget(numeric);
    this->layout()->addWidget(new QLabel(Application::identity()->hertz()));
    numeric->setMinimum(SLIDER_SCALE.getRange().getMin());
    numeric->setMaximum(SLIDER_SCALE.getRange().getMax() * 3);
    slider->setSingleStep(1);
    slider->setMinimum(SLIDER_SCALE.getDomain().getMin());
    slider->setMaximum(SLIDER_SCALE.getDomain().getMax());
    numeric->setSingleStep(0.01);
    connect(slider, SIGNAL(valueChanged(int)), SLOT(holdSlider(int)));
    connect(numeric, SIGNAL(valueChanged(double)), SLOT(holdSpinner(double)));
    this->setEnabled(false);
}

FEMViewer::FEMViewerFrequencyInput::~FEMViewerFrequencyInput()
{
}

void FEMViewer::FEMViewerFrequencyInput::setValue(double v) {
    numeric->setValue(v);
}

void FEMViewer::FEMViewerFrequencyInput::holdSlider(int v) {
    if (SLIDER_SCALE(v) != numeric->value()) {
        numeric->blockSignals(true);
        numeric->setValue(SLIDER_SCALE(v));
        numeric->blockSignals(false);
        emit valueChanged(SLIDER_SCALE(v));
    }
}

void FEMViewer::FEMViewerFrequencyInput::holdSpinner(double v) {
    if (SLIDER_SCALE[v] != slider->value()) {
        slider->blockSignals(true);
        slider->setValue(SLIDER_SCALE[v]);
        slider->blockSignals(false);
        emit valueChanged(v);
    }
}


void FEMViewer::FEMViewerFrequencyInput::changeEvent(QEvent * e) {
    if (e->type() == QEvent::EnabledChange) {
        for (auto& i : this->children()) {
            if (dynamic_cast<QWidget*>(i)) {
                static_cast<QWidget*>(i)->setEnabled(this->isEnabled());
            }
        }
    }
}

