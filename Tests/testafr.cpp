#include <QtTest/QTest>
#include "testafr.h"

#include "afr/afr.h"
#include "afr/afrarray.h"
#include "c2dchart.h"
#include "cgl.h"
#include "iostream"
#include <conio.h>
#include <QObject>


TestAfr::TestAfr(QObject *parent)
    : QObject(parent){

}


void TestAfr::findEigenFreqTest(){

    AFRArray examplArray;
    examplArray.read("../testFile/FRF_METEORIT.unv");
    AFR exampl(examplArray.average());
    CRealRange range(0,50);
    QCOMPARE((double)examplArray.size() ,38.0);
    QCOMPARE(exampl.findEigenFreq(range).frequency,31.4);
}

void TestAfr::damping(){

    AFRArray examplArray;
    examplArray.read("../testFile/FRF_METEORIT.unv");
    AFR exampl(examplArray.average());
    CRealRange range(0,50);
    QCOMPARE(exampl.damping(exampl.findEigenFreq(range).frequency),2.6585290179379393);
}
