#include <cstdio>
#include <QApplication>
#include "geometryform.h"
#include "geometrywidget.h"
#include "CGL/cparse.h"
#include <iostream>
#include <QJsonObject>
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
    //form->readBDF("C:\\Users\\NICK\\Downloads\\model.bdf");
    //form->readF06("C:\\Users\\NICK\\Downloads\\model.f06");
    //form->colorizeElements(form->modes().at(0).power());
    form->readBDF("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.bdf");
    form->readF06("C:\\Users\\NICK\\Downloads\\VVU developement\\Data\\METEORIT.f06");

    //solorize to power if it readed
    if (!form->modes().at(0).power().empty())
        form->colorizeElements(form->modes().at(0).power());

    QByteArray json("{\
                    \"FirstName\": \"John\",\
                    \"LastName\": \"Doe\",\
                    \"Age\": 43,\
                    \"Address\": {\
                        \"Street\": \"Downing Street 10\",\
                        \"City\": \"London\",\
                        \"Country\": \"Great Britain\"\
                    },\
                    \"Phone numbers\": [\
                        \"+44 1234567\",\
                        \"+44 2345678\"\
                    ]\
                }");
    QJsonObject arr;
    arr["val"] = "some";
    GeometryWidget w;
    w.setModel(form);
    qDebug() << "show";
    w.show();
    QFile f("tmp.mod");
    f.open(QFile::WriteOnly);
    QDataStream s(&f);
    //s << *form;
    f.close();
    f.open(QFile::ReadOnly);
    GeometryForm* form2(new GeometryForm);
    QDataStream r(&f);
    //r >> *form2;

    return a.exec();
}
