#include "viewertab.h"

#include <QModelIndex>
#include <QTreeView>
#include <QFile>
#include <QMessageBox>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QLabel>

#include <fem.h>

#include "viewerview.h"
#include "application.h"
#include "project.h"
#include "gui/toolbox/femviewer/femviewer.h"
#include "kernel/eigenmodesfounder.h"
#include <c3dcolumnchart.h>
#include <fem.h>
#include "gui/toolbox/macdisplay.h"
#include "gui/mainWindow/mainwindow.h"

ViewerTab::ViewerTab(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
    , femView(new ViewerView(this))
    , cascadeNode(new FEMViewer(this))
{
    this->connect(femView, &ViewerView::currentModeChanged, [this](int v) { cascadeNode->setMode(v); });
    this->connect(femView, &ViewerView::currentModelChanged, [this](int id) {
        try { cascadeNode->setModel(Application::project()->FEMList().at(id)); }
        catch(...) {
#ifndef QT_NO_DEBUG
            qFatal(QString(QString("setModel invalid id ") + QString::number(id)).toLocal8Bit().data());
#endif
        }
    });
    this->connect(femView, &ViewerView::MACPressed, [](int id, FEMModel::SharedEigenModesFounder p) {
        MACDisplay* chart(new MACDisplay(static_cast<MainWindow*>(Application::mainWindow())));
        chart->setData(Application::project()->toFEM(id)->getModes().getMAC());
        if (p) {
            p->connect(&*p, &EigenModesFounder::MACUpdated, [chart, p](){
                FEM* const model(p->model());
                chart->setData(model->getModes().getMAC());
            });
        }
        chart->connect(chart, &MACDisplay::closed, &MACDisplay::deleteLater);
        chart->show();
    });
    this->addWidget(femView);
    this->addWidget(cascadeNode);
    this->setSizes(QList<int>() << 300 << 1000);
}

ViewerTab::~ViewerTab()
{

}

void ViewerTab::acceptNewProject()
{
    femView->acceptNewProject();
    cascadeNode->setModel(Application::project()->FEMList().empty() ? 0 : Application::project()->FEMList().front());
}
