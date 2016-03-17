#include "femviewermagnitudeinput.h"
#include <cscale.h>
#include <QSlider>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>

#include "application.h"
#include "identity.h"

namespace {
typedef CScale<int, double> ToRealScale;
static const ToRealScale SLIDER_SCALE = ToRealScale(ToRealScale::InRange(0, 1000), ToRealScale::OutRange(-5.0, 5.0));
}

FEMViewer::FEMViewerMagnitudeInput::FEMViewerMagnitudeInput(QWidget *parent)
    : QFrame(parent)
    , slider(new QSlider(Qt::Horizontal, this))
{
    this->setLayout(new QHBoxLayout(this));
    this->layout()->addWidget(new QLabel(Application::identity()->tr("magnitude", "FEMViewer"), this));
    this->layout()->addWidget(slider);

    slider->setMinimum(SLIDER_SCALE.getDomain().getMin());
    slider->setMaximum(SLIDER_SCALE.getDomain().getMax());

    this->connect(slider, SIGNAL(valueChanged(int)), SLOT(holdSlider(int)));

    this->setDisabled(true);
}

FEMViewer::FEMViewerMagnitudeInput::~FEMViewerMagnitudeInput()
{
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

double FEMViewer::FEMViewerMagnitudeInput::getValue() const {
    return SLIDER_SCALE(slider->value());
}

void FEMViewer::FEMViewerMagnitudeInput::setValue(double val) {
    slider->setValue(SLIDER_SCALE[val]);
}

void FEMViewer::FEMViewerMagnitudeInput::holdSlider(int val) {
    emit valueChanged(SLIDER_SCALE(val));
}
