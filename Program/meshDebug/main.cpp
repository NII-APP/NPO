#include <fem.h>
#include <femwidget.h>
#include <QApplication>
#include <QDir>
#include <cparse.h>
#include <QDebug>
#include <QSpinBox>
#include <QVBoxLayout>
#include "mainwindow.h"

#define REREAD
int main(int argc, char** argv) {
    QApplication app(argc, argv);
    srand(QTime::currentTime().msec());

    qDebug() << "ecddddfrdddxfsvkrfrddddfrdddrfvfrfdfx4frfrrkttrfddr";
    FEM model;
    QFile f("model.dat");
#ifdef REREAD
    //model.read("C:\\Users\\BOPOHOB\\Downloads\\sim1-solution_1.bdf");
    //model.read("C:\\Users\\BOPOHOB\\Downloads\\sim1-solution_1 (1).f06");
    model.read("C:\\Users\\BOPOHOB\\Documents\\Qt\\work\\NPO\\Aaron.bdf");
    model.read("C:\\Users\\BOPOHOB\\Documents\\Qt\\work\\NPO\\Aaron.f06");
    f.open(QFile::WriteOnly);
    QDataStream out(&f);
    out << model;
    f.close();
#endif
    f.open(QFile::ReadOnly);
    QDataStream in(&f);
    in >> model;
    f.close();
\
    MainWindow w(model);
    w.show();

    return app.exec();
}
