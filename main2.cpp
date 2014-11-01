#include <cstdio>
#include <QApplication>
#include "geometryform.h"
#include "geometrywidget.h"
#include "CGL/cparse.h"
#include <iostream>
#include <QVariantMap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setlocale(LC_ALL,"RUS");

    QVariantMap map;
    map["sime val"] = 10;
    map["id"] = 0;
    map["name"] = "MAPA";
    map["id"] = 1;
    qDebug() << map;
    map.insert("name", 11);
    qDebug() << *map.find("name");
    qDebug() << map;

    printf("hello!\n");
    //Geometry* theory = new Geometry("C:\\Users\\NICK\\Downloads\\model.bdf");
    //Geometry* theory2 = new Geometry("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.bdf");
    //Geometry* practic = new Geometry("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.unv");
    GeometryForm* form = new GeometryForm;
    form->readBDF("C:\\Users\\samsung\\Desktop\\model\\Model.bdf");
    form->readF06("C:\\Users\\samsung\\Desktop\\model\\Model.f06");
    //form->colorizeElements(form->modes().at(0).power());
    //form->readBDF("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.bdf");
    //form->readF06("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.f06");

    GeometryWidget w;
    w.setModel(form);
    qDebug() << "show";
    w.show();
    QFile f("tmp.mod");
    f.open(QFile::WriteOnly);
    QDataStream s(&f);
    s << *form;
    //GLForm w2(theory2);
    //w2.show();

    qDebug() << "create mega matrux";
    CGL::Matrix mat(50, form->nodes().length());
    qDebug() << "fill it with random values";
    srand(22);
    for (int i = 0; i < mat.width(); ++i) {
        for (int j = 0; j < mat.height(); ++j) {
            mat[i][j] = rand() * 100.0 / rand();
        }
    }
   /* double** matrix;
    matrix = mat.asCMactix();
    qDebug() <<
                mat[5][4] << "==" << matrix[5][4];


*/
    return a.exec();
}
