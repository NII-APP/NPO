#include "pairstab.h"

#include <QDebug>

#include "application.h"
#include "identity.h"
#include "pairsview.h"
#include "gui/toolbox/femviewer/fempairviewer.h"
#include "gui/pairWizard/truncationwizard.h"
#include "project.h"
#include "gui/mainWindow/mainwindow.h"
#include "project/pairsmodel.h"

PairsTab::PairsTab(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
    , __view(new PairsView(this))
    , __viewer(new FEMPairViewer(this))
{
    this->addWidget(__view);
    this->addWidget(__viewer);
    this->setSizes(QList<int>() << 300 << 1000);

    connect(__view, &PairsView::addPairPressed, [this] () {
        FEMPair* const pair(TruncationWizard::exec(Application::mainWindow()));
        if (pair != nullptr) {
            __view->model()->insertPair(pair);
        }
    });
    connect(__view, &PairsView::currentPairChanged, [this] (int i) {
        __viewer->setPair(Application::project()->pairsList()[i]);
        __viewer->updateRelations();
    });
}

void PairsTab::acceptNewProject() {
    __view->acceptNewProject();
}
