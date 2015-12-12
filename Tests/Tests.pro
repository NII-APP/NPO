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

INCLUDEPATH += $$PWD/../Program/NPO
INCLUDEPATH += $$PWD/../Program/Mesh
INCLUDEPATH += $$PWD/../Program/CGL

SOURCES += ../Program/CGL/*.cpp
SOURCES += ../Program/Mesh/*.cpp
SOURCES += ../Program/Mesh/elements/*.cpp
SOURCES += ../Program/NPO/*.cpp
SOURCES += ../Program/NPO/engine/*.cpp

SOURCES += testafr.cpp \
    main.cpp

HEADERS += testafr.h
HEADERS += ../Program/CGL/*.h
HEADERS += ../Program/Mesh/*.h
HEADERS += ../Program/Mesh/elements/*.h
HEADERS += ../Program/NPO/*.h
HEADERS += ../Program/NPO/engine/*.h

DISTFILES += \
    testFile/FRF_METEORIT.unv
