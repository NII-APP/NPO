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

CONFIG(debug, debug|release): {
OBJECTS_DIR = $$PWD/../build/debug/main/obj
TARGET = main_d
}
else: {
OBJECTS_DIR = $$PWD/../build/release/main/obj
TARGET = main
}
DESTDIR = $$PWD/../binaries
RCC_DIR = $$PWD/../build/main/rcc
UI_DIR = $$PWD/../build/main/ui
MOC_DIR = $$PWD/../build/main/moc

SOURCES += main.cpp \
    engine/animationoptions.cpp \
    engine/geometrywidget.cpp \
    relationdialog.cpp \
    engine/form.cpp \
    engine/geometry.cpp \
    engine/geometryform.cpp \
    engine/geometrypair.cpp \
    engine/pseudoInverse.cpp \
    engine/core/finitelement.cpp \
    engine/core/hexa.cpp \
    engine/core/lines.cpp \
    engine/core/material.cpp \
    engine/core/quad.cpp \
    engine/core/shell.cpp \
    engine/core/tetra.cpp \
    engine/core/tria.cpp \
    application.cpp \
    identity.cpp \
    project.cpp \
    mainwindow.cpp \
    geometriesmodel.cpp \
    viewer.cpp \
    truncationtab.cpp \
    truncationwizard.cpp \
    maintabbar.cpp \
    engine/geometryobsoleteparse.cpp \
    engine/meshplace.cpp

HEADERS += \
    engine/animationoptions.h \
    engine/geometrywidget.h \
    relationdialog.h \
    engine/form.h \
    engine/geometry.h \
    engine/geometryform.h \
    engine/geometrypair.h \
    engine/pseudoInverse.h \
    engine/core/finitelement.h \
    engine/core/hexa.h \
    engine/core/lines.h \
    engine/core/material.h \
    engine/core/quad.h \
    engine/core/shell.h \
    engine/core/tetra.h \
    engine/core/tria.h \
    application.h \
    identity.h \
    project.h \
    mainwindow.h \
    geometriesmodel.h \
    viewer.h \
    truncationtab.h \
    truncationwizard.h \
    maintabbar.h \
    engine/meshplace.h


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
    main.json
