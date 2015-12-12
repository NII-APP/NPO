#include <QtTest/QTest>
#include "testafr.h"
#include "../NPO/engine/afr.h"
#include "../NPO/engine/afrarray.h"
#include "../CGL/c2dchart.h"
#include "../CGL/cgl.h"
#include "iostream"
#include <conio.h>


TestAfr::TestAfr(QObject *parent)
    : QObject(parent)
{
    qDebug() << "constructor2";
}

void TestAfr::findEigenFreqTest(){


    AFRArray examplArray;
    examplArray.read("C:\\Users\\Vladimir\\Documents\\Qt\\NPO\\Program\\Tests\\testFile\\FRF_METEORIT.unv");
    AFR exampl(examplArray.average());
    CRealRange range(0,50);
    qDebug() << "----------------" << exampl.findEigenFreq(range).frequency;
    QCOMPARE((double)examplArray.size() ,38.0);
    QCOMPARE(exampl.findEigenFreq(range).frequency,31.4);
}

void TestAfr::damping(){

    AFRArray examplArray;
    examplArray.read("C:\\Users\\Vladimir\\Documents\\Qt\\NPO\\Program\\Tests\\testFile\\FRF_METEORIT.unv");
    AFR exampl(examplArray.average());
    CRealRange range(0,50);
    QCOMPARE(exampl.damping(exampl.findEigenFreq(range).frequency),2.6585290179379393);
}
