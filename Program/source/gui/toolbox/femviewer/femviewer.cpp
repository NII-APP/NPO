#include "femviewer.h"

#include <QSpinBox>
#include <QToolBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QSvgRenderer>
#include <QAction>

#include <femwidget.h>
#include <fem.h>

#include "frequencyinput.h"
#include "modeinput.h"
#include "magnitudeinput.h"
#include "identity.h"
#include "application.h"
#include "identity.h"
#include "toolbar.h"

FEMViewer::FEMViewer(QWidget* parent)
    : FEMScreen(parent)
{
}

FEMViewer::~FEMViewer()
{
}

void FEMViewer::setModel(const FEM* m) const
{
    femWidget->setVisible(m != nullptr);
    femWidget->setData(m);
    updateToolBar();

    femWidget->resize(this->size());
}

void FEMViewer::setMode(const int m)
{
    toolbox->setMode(m);
}

void FEMViewer::updateToolBar() const
{
    toolbox->refresh();
}

void FEMViewer::colorize(int m)
{
    femWidget->colorize(m);
}

const FEM* FEMViewer::getModel() const
{
    if (!femWidget->getData().isEmpty()) {
        return femWidget->getData().first();
    } else {
        return nullptr;
    }
}

void FEMViewer::setProxyMode(const EigenMode& m)
{
    femWidget->setProxyMode(m);
    toolbox->setProxyModeState();
}
