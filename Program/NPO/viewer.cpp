#include "viewer.h"
#include <QListView>
#include <QFileDialog>
#include "geometriesmodel.h"
#include "femwidget.h"
#include "identity.h"
#include "application.h"
#include "project.h"
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <iostream>
#include "eigenmodes.h"

Viewer::Viewer(QWidget *parent, unsigned enabledItems)
    : QSplitter(Qt::Horizontal, parent)
    , geometryWidget(nullptr)
    , geometriesView(nullptr)
    , macChart(nullptr)
    , formSelector(nullptr)
    , formLabel(nullptr)
    , formSubLabel(nullptr)
    , form(nullptr)
    , colorizeBundle(true)
{
}

Viewer::~Viewer()
{

}

