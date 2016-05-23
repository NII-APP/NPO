#include "relationmodeinput.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

#include "application.h"
#include "identity.h"

RelationModeInput::RelationModeInput(QWidget *parent)
    : QFrame(parent)
    , left(new QSpinBox(this))
    , right(new QSpinBox(this))
{
    this->setLayout(new QHBoxLayout(this));
    this->layout()->addWidget(new QLabel(Application::identity()->tr("mode", "FEMViewer"), this));
    this->layout()->addWidget(left);
    this->layout()->addWidget(right);

}

RelationModeInput::~RelationModeInput()
{

}
