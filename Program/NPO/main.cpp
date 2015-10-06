#include "application.h"
#include "mainwindow.h"
#include <QDebug>

#include <c2dchart.h>
#include <cchartdata.h>
#include <cdimensioninterval.h>

#include "modesidentificationwizard.h"
#include <fem.h>
#include "femviewer.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    setlocale(LC_ALL,"RUS");

    MainWindow window;
    a.setMainWindow(&window);

    window.showMaximized();

    return a.exec();
}
