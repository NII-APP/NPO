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
    this->connect(femView, SIGNAL(currentModelChanged(int)), SLOT(setModel(int)));
    this->connect(femView, SIGNAL(importModesPressed(int)), SLOT(addModes(int)));
    this->connect(femView, SIGNAL(currentModeChanged(int, int)), SLOT(setMode(int)));
    this->addWidget(femView);
    this->addWidget(femWidget);
    this->setSizes(QList<int>() << 200 << 1000);
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

void ViewerTab::setModel(int id) {
    qDebug() << "set model" << id;
    try {
        setModel(Application::project()->modelsList().at(id));
    } catch(...) {
        Q_ASSERT("setModel invalid id");
    }
}

void ViewerTab::setMode(int v) {
    femWidget->setMode(v);
}

void ViewerTab::addModes(int meshId) {
    FEM* const fem(Application::project()->modelsList().at(meshId));
    QString file(Application::identity()->choseModesFile());
    if (!QFile::exists(file)) {
        return;
    }
    fem->read(file);
    femWidget->updateToolBar();
    femView->reset();
}
