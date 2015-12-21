#include "application.h"
#include "mainwindow.h"

#include <QFile>
#include <QDebug>

#include <c2dchart.h>
#include <cchartdata.h>
#include <cdimensioninterval.h>

#include "filenameedit.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);

    qDebug() << "xyu";
    MainWindow w;

#ifndef QT_NO_DEBUG
    w.show();
#else
    w.showMaximized();
#endif

    return a.exec();
}
