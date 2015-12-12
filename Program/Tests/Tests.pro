#-------------------------------------------------
#
# Project created by QtCreator 2015-12-09T22:24:03
#
#-------------------------------------------------


QT       += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../CGL/CGL.pri)
include(../Mesh/Mesh.pri)
include(../NPO/NPO.pri)

TARGET = Tests

TEMPLATE = lib
CONFIG += staticlib

DEFINES += TESTS_LIBRARY

SOURCES += testafr.cpp

HEADERS += testafr.h\
        tests_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    testFile/FRF_METEORIT.unv
