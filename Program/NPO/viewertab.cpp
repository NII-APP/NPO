#include "viewertab.h"

#include <QModelIndex>
#include <QTreeView>
#include <QFile>
#include <QMessageBox>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QLabel>

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
#include "modesidentificationwizard.h"

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
    this->connect(femView, SIGNAL(modesIdentificationPressed(int)), SLOT(identificateModes(int)));
    this->connect(femView,&ViewerView::calcModes, [this](int v) {
        QMessageBox* const hold(new QMessageBox(this));
        hold->setModal(true);
        hold->setStandardButtons(QMessageBox::NoButton);
        hold->setText("Ща посчитаю");
        QEventLoop* const loop(new QEventLoop(hold));
        FEMProcessor p(Application::nonConstProject()->modelsList()[v], hold);
        connect(&p, &FEMProcessor::finished, [loop](){
            loop->exit();
        });
        connect(hold, &QMessageBox::finished, [loop]() { loop->exit(); });
        hold->show();
        p.calculateModes();
        loop->exec();
        hold->deleteLater();
    });
    this->addWidget(femView);
    this->addWidget(cascadeNode);
    this->setSizes(QList<int>() << 300 << 1000);
}

ViewerTab::~ViewerTab()
{

}

void ViewerTab::showMAC(int id) {
    const FEM* const model(Application::project()->modelsList().at(id));
    MACMap::iterator i(MACs.find(model));
    if (i != MACs.end()) {
        i.value()->raise();
        return;
    }
    MACDisplay* chart(new MACDisplay(0));
    chart->setData(Application::project()->modelsList().at(id)->getModes().getMAC());
    MACs.insert(model, chart);

    connect(chart, SIGNAL(closed()), chart, SLOT(deleteLater()));
    connect(chart, SIGNAL(closed()), this, SLOT(forgetMACWidget()));
    if (processors.contains(model)) {
        connect(processors[model], SIGNAL(MACUpdated()), this, SLOT(updateMACWidget()));
    }

    chart->show();
}

void ViewerTab::identificateModes(int meshId) {
    ModesIdentificationWizard::identifyModes(Application::project()->modelsList().at(meshId), reinterpret_cast<QWidget*>(Application::mainWindow()));
}

void ViewerTab::updateMACWidget() {
    FEMProcessor* sender(dynamic_cast<FEMProcessor*>(QObject::sender()));
    if (sender == 0) {
        return;
    }
    const FEM* const model(processors.key(sender));
    if (MACs.contains(model)) {
        MACs[model]->setData(model->getModes().getMAC());
    }
}

void ViewerTab::forgetMACWidget() {
    MACDisplay* sender(dynamic_cast<MACDisplay*>(QObject::sender()));
    const FEM* const p(MACs.key(sender, 0));
    if (p) {
        MACs.remove(p);
    }
}
void ViewerTab::forgetFEMProcessor() {
    const FEM* const p(processors.key(dynamic_cast<FEMProcessor*>(QObject::sender()), 0));
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
    Application::nonConstProject()->pushModel(fem);
    femView->update();
    femView->setCurrentIndex(femView->model()->index(static_cast<int>(Application::project()->modelsList().size()) - 1, 0));
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
    setModel(Application::project()->modelsList().empty() ? 0 : Application::project()->modelsList().front());
}

void ViewerTab::addModes(int meshId) {
    QString file(Application::identity()->choseModesFile());
    if (!QFile::exists(file)) {
        return;
    }
    FEM* model(Application::nonConstProject()->modelsList().at(meshId));
    FEMProcessor* p(new FEMProcessor(model, this));
    connect(p, SIGNAL(modelReaded()), cascadeNode, SLOT(update()));
    connect(p, SIGNAL(modelReaded()), femView, SLOT(updateCurrentModel()));
    connect(p, SIGNAL(finished()), p, SLOT(deleteLater()));
    connect(p, SIGNAL(finished()), this, SLOT(forgetFEMProcessor()));
    processors.insert(model, p);
    if (MACs.contains(model)) {
        connect(p, SIGNAL(MACUpdated()), this, SLOT(updateMACWidget()));
    }
    p->read(file);
}
