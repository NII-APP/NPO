#include "application.h"
#include "mainwindow.h"
#include <QDebug>

#include <c2dchart.h>
#include <cchartdata.h>
#include <cdimensioninterval.h>

#include "filenameedit.h"

int main(int argc, char *argv[])
{
    qDebug() << "app";
    Application a(argc, argv);
    setlocale(LC_ALL,"RUS");
    qDebug() << "main wrddsxdindow";

    MainWindow w;

#ifndef QT_NO_DEBUG
    w.show();
#else
    w.showMaximized();
#endif

    return a.exec();
}
