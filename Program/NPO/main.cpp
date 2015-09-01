#include "application.h"
#include "mainwindow.h"
#include "cmatrix.h"
#include <QDebug>
#include "c2dchart.h"
#include "cdimensionarray.h"
#include "cdimensioninterval.h"
#include <QDir>
#include <femwidget.h>
#include "femviewer.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    setlocale(LC_ALL,"RUS");

    QGLFormat fmt(QGLFormat::defaultFormat());
    fmt.setSampleBuffers(true);
    fmt.setSamples(2);
    QGLFormat::setDefaultFormat(fmt);

    MainWindow window;
    window.showMaximized();

    a.setMainWindow(&window);
    return a.exec();
}
