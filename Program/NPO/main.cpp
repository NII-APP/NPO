#include "application.h"
#include "mainwindow.h"
#include <QDebug>

#include "pairviewer.h"
#include "engine/fempair.h"
#include "project.h"
#include <QSettings>

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    setlocale(LC_ALL,"RUS");

    Application::nonConstProject()->load(QSettings().value("project/location").value<QString>());

    qDebug() << "V";
    PairViewer vMeansVendetta;
    vMeansVendetta.setData(new FEMPair(Application::project()->modelsList().front(), Application::project()->modelsList().at(1)));
    vMeansVendetta.show();

    return a.exec();
}
