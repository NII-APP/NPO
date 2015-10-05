#include "application.h"
#include "mainwindow.h"
#include <QDebug>

#include <c2dchart.h>
#include <cchartdata.h>
#include <cdimensioninterval.h>

#include "modesidentificationwizard.h"
#include <fem.h>

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    setlocale(LC_ALL,"RUS");

/*    MainWindow window;
    a.setMainWindow(&window);*/
    FEM* fem(new FEM);
    fem->read("C:\\Users\\username\\Downloads\\geom_Aaron.unv");
    ModesIdentificationWizard::identifyModes(fem);


    //window.showMaximized();

    a.quit();
    return 0;
    return a.exec();
}
