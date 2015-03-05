#include "application.h"
#include "mainwindow.h"
#include "engine/CGL/cmatrix.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    setlocale(LC_ALL,"RUS");

    MainWindow window;
    window.showMaximized();

    return a.exec();
}
