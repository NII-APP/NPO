#include "femviewerfrequencyinput.h"
#include <QDoubleSpinBox>
#include <QSlider>
#include <QHBoxLayout>
#include <QLabel>

const FEMViewerFrequencyInput::ToRealScale FEMViewerFrequencyInput::SLIDER_SCALE =
        FEMViewerFrequencyInput::ToRealScale(FEMViewerFrequencyInput::ToRealScale::InRange(0, 99),
                                             FEMViewerFrequencyInput::ToRealScale::OutRange(0.2, 5.0));

FEMViewerFrequencyInput::FEMViewerFrequencyInput(QWidget* parent)
    : QWidget(parent)
    , numeric(new QDoubleSpinBox(this))
    , slider(new QSlider(Qt::Horizontal, this)) {
    this->setLayout(new QHBoxLayout(this));
    this->layout()->addWidget(new QLabel(this->parent()->tr("Frequency"), this));
    this->layout()->addWidget(slider);
    this->layout()->addWidget(numeric);
    numeric->setMinimum(SLIDER_SCALE.getRange().getMin());
    numeric->setMaximum(SLIDER_SCALE.getRange().getMax());
    connect(slider, SIGNAL(valueChanged(int)), SLOT(holdSlider(int)));
    connect(numeric, SIGNAL(valueChanged(double)), SLOT(holdSpinner(double)));
}

void FEMViewerFrequencyInput::setValue(double v) {
    numeric->setValue(v);
}

void FEMViewerFrequencyInput::holdSlider(int v) {
    qDebug() << "slider" << SLIDER_SCALE(v) << numeric->value() << v << SLIDER_SCALE;
    if (SLIDER_SCALE(v) != numeric->value()) {
        numeric->setValue(SLIDER_SCALE(v));
        emit valueChanged(SLIDER_SCALE(v));
    }
}

void FEMViewerFrequencyInput::holdSpinner(double v) {
    qDebug() << "spinner" << SLIDER_SCALE[v] << numeric->value() << SLIDER_SCALE;
    if (SLIDER_SCALE[v] != slider->value()) {
        slider->setValue(SLIDER_SCALE[v]);
        emit valueChanged(v);
    }
}

