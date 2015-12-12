#include <QApplication>
#include <QtTest/QtTest>
#include "testafr.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QTest::qExec(new TestAfr);

    return a.exec();
}
