#include "modeinput.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <exception>
#include <QEvent>

#include <fem.h>
#include <eigenmodes.h>

#include "application.h"
#include "identity.h"
#include "femwidget.h"

ModeInput::ModeInput(FEMWidget *viewer, QWidget* parent)
    : QFrame(parent)
    , parentViewer(viewer)
    , hzValue(new QLabel(this))
    , value(new QSpinBox(this))
{
    this->setLayout(new QHBoxLayout(this));
    this->layout()->addWidget(new QLabel(Application::identity()->tr("mode", "FEMViewer"), this));
    this->layout()->addWidget(value);
    this->layout()->addWidget(hzValue);

    this->connect(value, SIGNAL(valueChanged(int)), SLOT(holdValue(int)));

    updateValueBounds(0);
}

ModeInput::~ModeInput() { }

void ModeInput::holdValue(int v) {
    try {
        const QList<const FEM*> models(parentViewer->getData());
        if (models.size() == 1) {
            hzValue->setText(("%1 " + Application::identity()->tr("hertz")).arg(models.first()->getModes().at(v - 1).frequency()));
            emit valueChanged(getValue());
        } else {
            throw std::exception();
        }
    } catch(...) {
        updateValueBounds();
    }
}

void ModeInput::updateValueBounds() {
    const QList<const FEM*> models(parentViewer->getData());
    if (models.size() == 1) {
        const FEM* const m(models.first());
        updateValueBounds(m ? static_cast<int>(m->getModes().size()) : 0);
    }
}

void ModeInput::updateValueBounds(int modesCount) {
    if (modesCount <= 0) {
        this->setDisabled(true);
    } else {
        this->setDisabled(false);
        value->setMinimum(1);
        value->setMaximum(modesCount);
    }

}

void ModeInput::changeEvent(QEvent * e) {
    if (e->type() == QEvent::EnabledChange) {
        value->setEnabled(this->isEnabled());
        hzValue->setEnabled(this->isEnabled());
    }
}

int ModeInput::getValue() const {
    return value->value() - 1;
}
void ModeInput::setValue(int value) {
    this->value->setValue(value + 1);
    emit valueChanged(getValue());
}
