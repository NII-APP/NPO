#include "viewertab.h"

#include <QModelIndex>
#include <QTreeView>
#include <QFile>

#include <fem.h>

#include "viewermodel.h"
#include "viewerview.h"
#include "application.h"
#include "project.h"
#include "femviewer.h"

ViewerTab::ViewerTab(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
    , femView(new ViewerView(this))
    , femWidget(new FEMViewer(this))
{
    femView->setModel(new ViewerModel(Application::project(), this));
    this->connect(femView, SIGNAL(addModelPressed()), SLOT(addModel()));
    this->addWidget(femView);
    this->addWidget(femWidget);
}

ViewerTab::~ViewerTab()
{

}

void ViewerTab::addModel() {
    FEM* fem(new FEM);;
    QString bdf(Application::identity()->choseModelFile());
    if (!QFile::exists(bdf)) {
        return;
    }
    fem->read(bdf);
    setModel(fem);
    Application::project()->pushModel(fem);
    femView->reset();
}

void ViewerTab::setModel(const FEM* model) {
    femWidget->setModel(model);
}

void ViewerTab::listPatrol(QModelIndex) {}
