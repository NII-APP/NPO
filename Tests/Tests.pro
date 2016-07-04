#-------------------------------------------------
#
# Project created by QtCreator 2015-12-09T22:24:03
#
#-------------------------------------------------


QT       += opengl widgets testlib\
            svg core gui\
            winextras

TARGET = tests
TEMPLATE = app

include(../Program/source/source.pri)

INCLUDEPATH += $$PWD/../Program/NPO
INCLUDEPATH += $$PWD/../Program/Mesh
INCLUDEPATH += $$PWD/../Program/CGL

SOURCES += testafr.cpp \
    main.cpp

HEADERS += testafr.h


RESOURCES += \
    ../Program/res.qrc \
    testres.qrc


DISTFILES += \
    ../Program/resource/css.css \
    ../Program/resource/main.json
