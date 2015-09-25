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
#include "viewernode.h"
#include "femprocessor.h"
#include <c3dcolumnchart.h>
#include <fem.h>
#include "macdisplay.h"

ViewerTab::ViewerTab(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
    , femView(new ViewerView(this))
    , cascadeNode(new ViewerNode(this))
{
    this->connect(femView, SIGNAL(addModelPressed()), SLOT(addModel()));
    this->connect(femView, SIGNAL(currentModelChanged(int)), SLOT(setModel(int)));
    this->connect(femView, SIGNAL(importModesPressed(int)), SLOT(addModes(int)));
    this->connect(femView, SIGNAL(currentModeChanged(int, int)), SLOT(setMode(int)));
    this->connect(femView, SIGNAL(MACPressed(int)), SLOT(showMAC(int)));
    this->addWidget(femView);
    this->addWidget(cascadeNode);
    this->setSizes(QList<int>() << 300 << 1000);
}

ViewerTab::~ViewerTab()
{

}

void ViewerTab::showMAC(int id) {
    FEM* const model(Application::project()->modelsList().at(id));
    MACMap::iterator i(MACs.find(model));
    if (i != MACs.end()) {
        i.value()->raise();
        return;
    }
    MACDisplay* chart(new MACDisplay(0));
    chart->setData(Application::project()->modelsList().at(id)->getModes().getMAC());
    MACs.insert(model, chart);

    connect(chart, SIGNAL(closed()), chart, SLOT(deleteLater()));
    connect(chart, SIGNAL(closed()), this, SLOT(fogiveMACWidget()));
    if (processors.contains(model)) {
        connect(processors[model], SIGNAL(MACUpdated()), this, SLOT(updateMACWidget()));
    }

    chart->show();
}

void ViewerTab::updateMACWidget() {
    FEMProcessor* sender(dynamic_cast<FEMProcessor*>(QObject::sender()));
    if (sender == nullptr) {
        return;
    }
    FEM* model(processors.key(sender));
    MACs[model]->setData(model->getModes().getMAC());
}

void ViewerTab::fogiveMACWidget() {
    MACDisplay* sender(dynamic_cast<MACDisplay*>(QObject::sender()));
    FEM* p(MACs.key(sender, nullptr));
    if (p) {
        MACs.remove(p);
    }
}
void ViewerTab::fogiveFEMProcessor() {
    FEM* p(processors.key(dynamic_cast<FEMProcessor*>(QObject::sender()), nullptr));
    if (p) {
        processors.remove(p);
    }
}

void ViewerTab::addModel() {
    FEM* fem(new FEM);
    QString bdf(Application::identity()->choseModelFile());
    if (!QFile::exists(bdf)) {
        return;
    }
    fem->read(bdf);
    setModel(fem);
    Application::nonConstProject()->pushModel(fem);
    femView->reset();
}

void ViewerTab::setModel(const FEM* model) {
    cascadeNode->setModel(model);
}

void ViewerTab::setModel(int id) {
    try {
        setModel(Application::project()->modelsList().at(id));
    } catch(...) {
        qFatal("setModel invalid id");
    }
}

void ViewerTab::setMode(int v) {
    cascadeNode->setMode(v);
}

void ViewerTab::acceptNewProject() {
    femView->acceptNewProject();
    setModel(Application::project()->modelsList().empty() ? nullptr : Application::project()->modelsList().front());
}

void ViewerTab::addModes(int meshId) {
    QString file(Application::identity()->choseModesFile());
    if (!QFile::exists(file)) {
        return;
    }
    FEM* model(Application::project()->modelsList().at(meshId));
    FEMProcessor* p(new FEMProcessor(model, this));
    connect(p, SIGNAL(modelReaded()), cascadeNode, SLOT(update()));
    connect(p, SIGNAL(modelReaded()), femView, SLOT(update()));
    connect(p, SIGNAL(finished()), p, SLOT(deleteLater()));
    connect(p, SIGNAL(finished()), this, SLOT(fogiveFEMProcessor()));
    processors.insert(model, p);
    if (MACs.contains(model)) {
        connect(p, SIGNAL(MACUpdated(const CGL::CMatrix&)), MACs[model], SLOT(setData(const CGL::CMatrix&)));
    }
    p->read(file);
}
