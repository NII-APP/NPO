#include "magnitudeinput.h"

#include <cassert>
#include <QSlider>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QPainter>

#include <cscale.h>

#include "application.h"
#include "identity.h"

namespace {
typedef CScale<int, double> ToRealScale;
static const ToRealScale SLIDER_SCALE = ToRealScale(ToRealScale::InRange(0, 1000), ToRealScale::OutRange(-5.0, 5.0));
}

MagnitudeInput::MagnitudeInput(QWidget *parent)
    : QFrame(parent)
    , slider(new QSlider(Qt::Horizontal, this))
{
    this->setLayout(new QHBoxLayout(this));
    this->layout()->addWidget(([this]()->QWidget* {
                                   QLabel* const l(new QLabel(this));
                                   static const QPixmap icon(([this]()->QPixmap{
                                       QPixmap m(":/media/resource/images/magnitude.png");
                                       return m.scaled(this->height(), this->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                                   })());
                                   l->setToolTip(Application::identity()->tr("magnitude", "FEMViewer"));
                                   l->setPixmap(icon);
                                   return l;
    })());
    this->setMinimumWidth(200);
    this->layout()->addWidget(slider);

    slider->setMinimum(SLIDER_SCALE.getDomain().getMin());
    slider->setMaximum(SLIDER_SCALE.getDomain().getMax());

    this->connect(slider, SIGNAL(valueChanged(int)), SLOT(holdSlider(int)));

    this->setDisabled(true);
}

MagnitudeInput::~MagnitudeInput()
{
}

void MagnitudeInput::resizeEvent(QResizeEvent* e)
{
    QFrame::resizeEvent(e);
}

void MagnitudeInput::changeEvent(QEvent * e) {
    if (e->type() == QEvent::EnabledChange) {
        for (auto& i : this->children()) {
            if (dynamic_cast<QWidget*>(i)) {
                static_cast<QWidget*>(i)->setEnabled(this->isEnabled());
            }
        }
    }
}

double MagnitudeInput::getValue() const {
    return SLIDER_SCALE(slider->value());
}

void MagnitudeInput::setValue(double val) {
    slider->setValue(SLIDER_SCALE[val]);
}

void MagnitudeInput::holdSlider(int val) {
    emit valueChanged(SLIDER_SCALE(val));
}
