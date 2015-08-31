#include "femviewermagnitudeinput.h"
#include <cscale.h>
#include <QSlider>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>

namespace {
typedef CScale<int, double> ToRealScale;
static const ToRealScale SLIDER_SCALE = ToRealScale(ToRealScale::InRange(0, 1000), ToRealScale::OutRange(0.0, 5.0));
}

FEMViewer::FEMViewerMagnitudeInput::FEMViewerMagnitudeInput(QWidget *parent)
    : QFrame(parent)
    , slider(new QSlider(Qt::Horizontal, this))
{
    this->setLayout(new QHBoxLayout(this));
    this->layout()->addWidget(new QLabel(this->parent()->parent()->tr("Magnitude"), this));
    this->layout()->addWidget(slider);

    slider->setMinimum(SLIDER_SCALE.getDomain().getMin());
    slider->setMaximum(SLIDER_SCALE.getDomain().getMax());

    this->connect(slider, SIGNAL(valueChanged(int)), SLOT(holdSlider(int)));

    this->setDisabled(true);
}

void FEMViewer::FEMViewerMagnitudeInput::changeEvent(QEvent * e) {
    if (e->type() == QEvent::EnabledChange) {
        for (auto& i : this->children()) {
            if (dynamic_cast<QWidget*>(i)) {
                static_cast<QWidget*>(i)->setEnabled(this->isEnabled());
            }
        }
    }
}

void FEMViewer::FEMViewerMagnitudeInput::setValue(double val) {
    slider->setValue(SLIDER_SCALE[val]);
}

void FEMViewer::FEMViewerMagnitudeInput::holdSlider(int val) {
    emit valueChanged(SLIDER_SCALE(val));
}