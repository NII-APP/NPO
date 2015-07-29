#include "application.h"
#include "mainwindow.h"
#include "cmatrix.h"
#include <QDebug>
#include "c2dchart.h"
#include "cdimensionarray.h"
#include "cdimensioninterval.h"


int main(int argc, char *argv[])
{
    Application a(argc, argv);
    setlocale(LC_ALL,"RUS");

    MainWindow window;
    window.showMaximized();

    a.setMainWindow(&window);
    return a.exec();
}
