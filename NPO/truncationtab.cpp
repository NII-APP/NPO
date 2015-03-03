#include "truncationtab.h"
#include <QToolBar>
#include <QPushButton>
#include "truncationwizard.h"

TruncationTab::TruncationTab(QWidget *parent) : QMainWindow(parent)
{
    bar = new QToolBar(this);
    QPushButton* pb;
    bar->addWidget(pb = new QPushButton(this));
    this->connect(pb, SIGNAL(clicked()), SLOT(addPair()));

    this->addToolBar(bar);
}

void TruncationTab::addPair()
{
    TruncationWizard::exec(this);
}

TruncationTab::~TruncationTab()
{

}

