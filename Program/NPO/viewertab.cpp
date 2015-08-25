#include "viewertab.h"

#include <QModelIndex>
#include <QTreeView>

#include "viewermodel.h"
#include "viewerview.h"
#include "application.h"
#include "femviewer.h"

ViewerTab::ViewerTab(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
    , femView(new ViewerView(this))
{
    femView->setModel(new ViewerModel(Application::project(), this));
    this->addWidget(femView);
    this->addWidget(new FEMViewer(this));
}

ViewerTab::~ViewerTab()
{

}


void ViewerTab::listPatrol(QModelIndex) {}
