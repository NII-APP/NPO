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
#include "gui/toolbox/femviewer/femviewer.h"
#include "femprocessor.h"
#include <c3dcolumnchart.h>
#include <fem.h>
#include "gui/toolbox/macdisplay.h"

ViewerTab::ViewerTab(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
    , femView(new ViewerView(this))
    , cascadeNode(new FEMViewer(this))
{
    this->connect(femView, SIGNAL(currentModeChanged(int, int)), SLOT(setMode(int)));
    this->connect(femView, SIGNAL(currentModelChanged(int)), SLOT(setModel(int)));
    this->connect(femView, &ViewerView::MACPressed, [this](int id, FEMProcessor* p) {
        MACDisplay* chart(new MACDisplay(this));
        chart->setData(Application::project()->toFEM(id)->getModes().getMAC());
        if (p) {
            connect(p, &FEMProcessor::MACUpdated, [chart, p](){
                FEM* const model(p->model());
                chart->setData(model->getModes().getMAC());
            });
        }
        chart->connect(chart, &MACDisplay::closed, &MACDisplay::deleteLater);
        chart->show();
    });
    this->connect(femView, &ViewerView::calcModes, [this](int v) {
        QMessageBox* const hold(new QMessageBox(this));
        hold->setModal(true);
        hold->setStandardButtons(QMessageBox::NoButton);
        hold->setText("Ща посчитаю");
        QEventLoop* const loop(new QEventLoop(hold));
        FEMProcessor p(Application::nonConstProject()->FEMList()[v], hold);
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

void ViewerTab::setModel(const FEM* model) {
    cascadeNode->setModel(model);
}

void ViewerTab::setModel(int id) {
    try {
        setModel(Application::project()->FEMList().at(id));
    } catch(...) {
        qFatal("setModel invalid id");
    }
}

void ViewerTab::setMode(int v) {
    cascadeNode->setMode(v);
}

void ViewerTab::acceptNewProject() {
    femView->acceptNewProject();
    setModel(Application::project()->FEMList().empty() ? 0 : Application::project()->FEMList().front());
}
