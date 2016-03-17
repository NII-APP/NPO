#include "pairstab.h"

#include <QDebug>

#include "application.h"
#include "identity.h"
#include "pairsview.h"
#include "pairviewer.h"
#include "truncationwizard.h"

PairsTab::PairsTab(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
    , __view(new PairsView(this))
    , __viewer(new PairViewer(this))
{
    this->addWidget(__view);
    this->addWidget(__viewer);
    this->setSizes(QList<int>() << 300 << 1000);

    connect(__view, SIGNAL(addPairPressed()), SLOT(addPair()));
}

void PairsTab::addPair() {
    FEMPair* p(TruncationWizard::exec(this));
    delete p;
}

void PairsTab::acceptNewProject() {
    __view->acceptNewProject();
}
