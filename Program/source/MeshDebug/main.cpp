#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include "../NPO/engine/afrarray.h"
#include "../Mesh/fem.h"
#include "../Mesh/femwidget.h"

int main(int argc, char** argv) {
    QApplication a(argc, argv);

    FEM* model(new FEM);
    model->read("C:\\Users\\BOPOHOB\\Downloads\\Geom_METEORIT.unv");
    AFRArray arr;
    arr.read("C:\\Users\\BOPOHOB\\Downloads\\FRF_10.unv");
    qDebug() << QDir(":codes/").entryInfoList().first().fileName();

    FEMWidget w;
    w.setData(model);
    w.show();

    w.setProxyMode(arr.getMode(40.0));

    return a.exec();
}
