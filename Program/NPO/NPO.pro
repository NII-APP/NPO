#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T17:42:41
# then we go hard
#
#-------------------------------------------------

QT       += core opengl gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NPO

TEMPLATE = app

include(../CGL/CGL.pri)
include(../Mesh/Mesh.pri)

CONFIG(debug, debug|release): {
OBJECTS_DIR = $$PWD/../build/debug/main/obj
TARGET = main_d
}
else: {
OBJECTS_DIR = $$PWD/../build/release/main/obj
TARGET = main
}
DESTDIR = $$PWD/../build/binaries
RCC_DIR = $$PWD/../build/main/rcc
UI_DIR = $$PWD/../build/main/ui
MOC_DIR = $$PWD/../build/main/moc

SOURCES += main.cpp \
    relationdialog.cpp \
    engine/geometrypair.cpp \
    engine/pseudoInverse.cpp \
    application.cpp \
    identity.cpp \
    project.cpp \
    mainwindow.cpp \
    geometriesmodel.cpp \
    truncationtab.cpp \
    truncationwizard.cpp \
    maintabbar.cpp \
    viewertab.cpp \
    viewermodel.cpp

HEADERS += \
    relationdialog.h \
    engine/geometrypair.h \
    engine/pseudoInverse.h \
    application.h \
    identity.h \
    project.h \
    mainwindow.h \
    geometriesmodel.h \
    truncationtab.h \
    truncationwizard.h \
    maintabbar.h \
    viewertab.h \
    viewermodel.h

exists( engine/pyParse/BDFEntity.cpp ) {
    message( "Configuring with new parser" )
    SOURCES += engine/pyParse/BDFEntity.cpp
    HEADERS += engine/pyParse/BDFEntity.h
    DEFINES += "PyBDF"
}

OTHER_FILES += \
    summator.vert

RESOURCES += \
    res.qrc

DISTFILES += \
    css.css \
    main.json \
    readBDF.py
