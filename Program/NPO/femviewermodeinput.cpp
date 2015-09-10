#include "femviewermodeinput.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <exception>
#include <QEvent>

#include <fem.h>
#include <eigenmodes.h>

#include "application.h"
#include "identity.h"

FEMViewer::FEMViewerModeInput::FEMViewerModeInput(FEMViewer *viewer, QWidget* parent)
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


void FEMViewer::FEMViewerModeInput::holdValue(int v) {
    try {
        if (static_cast<FEMViewer*>(this->parentViewer)->getModel()) {
            hzValue->setText(("%1 " + Application::identity()->tr("hertz")).arg(static_cast<FEMViewer*>(this->parentViewer)->getModel()->getModes().at(v).frequency()));
            emit valueChanged(getValue());
        } else {
            throw std::exception();
        }
    } catch(...) {
        updateValueBounds();
    }
}

void FEMViewer::FEMViewerModeInput::updateValueBounds() {
    updateValueBounds(static_cast<int>(static_cast<FEMViewer*>(this->parentViewer)->getModel()->getModes().size()));
}

void FEMViewer::FEMViewerModeInput::updateValueBounds(int modesCount) {
    if (modesCount <= 0) {
        this->setDisabled(true);
    } else {
        this->setDisabled(false);
        value->setMinimum(1);
        value->setMaximum(modesCount);
    }

}

void FEMViewer::FEMViewerModeInput::changeEvent(QEvent * e) {
    if (e->type() == QEvent::EnabledChange) {
        value->setEnabled(this->isEnabled());
        hzValue->setEnabled(this->isEnabled());
    }
}

int FEMViewer::FEMViewerModeInput::getValue() const {
    return value->value() - 1;
}
void FEMViewer::FEMViewerModeInput::setValue(int value) {
    this->value->setValue(value + 1);
    emit valueChanged(getValue());
}
