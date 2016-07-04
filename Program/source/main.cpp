#include "application.h"
#include "gui/mainWindow/mainwindow.h"

#include <QFile>
#include <QDebug>

#include <c2dchart.h>
#include <cchartdata.h>
#include <cdimensioninterval.h>

#include "gui/toolbox/filenameedit.h"
#include "gui/pairWizard/truncationwizard.h"
#include "project.h"
#include "fem.h"
#include "kernel/modelupdate.h"
#include "fempair.h"
#include "kernel/igofesolver.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);

    MainWindow w;

#ifndef QT_NO_DEBUG
    w.show();
#else
    w.showMaximized();
#endif
    return a.exec();
}
