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
#include "kernel/modelrefining.h"
#include "fempair.h"
#include "kernel/igofesolver.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);


#ifndef SANDBOX
    MainWindow w;

#ifndef QT_NO_DEBUG
    w.show();
#else
    w.showMaximized();
#endif
#endif

    return a.exec();
}
