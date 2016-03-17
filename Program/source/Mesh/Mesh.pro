#-------------------------------------------------
#
# Project created by QtCreator 2015-07-20T20:25:56
#
#-------------------------------------------------

QT       += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14

CONFIG(debug, debug|release): {
OBJECTS_DIR = $$PWD/../build/debug/mesh/obj
TARGET = mesh_d
}
else: {
OBJECTS_DIR = $$PWD/../build/release/mesh/obj
TARGET = mesh
}
DESTDIR = $$PWD/../build/binaries
RCC_DIR = $$PWD/../build/app/rcc
UI_DIR = $$PWD/../build/app/ui
MOC_DIR = $$PWD/../build/app/moc

include(../CGL/CGL.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    Mesh.pri

SOURCES += \
    elements/finitelement.cpp \
    elements/hexa.cpp \
    elements/lines.cpp \
    elements/quad.cpp \
    elements/tetra.cpp \
    elements/tria.cpp \
    material.cpp \
    shell.cpp \
    eigenmode.cpp \
    eigenmodes.cpp \
    fem.cpp \
    fembdfparser.cpp \
    femwidget.cpp \
    bdfentity.cpp

HEADERS += \
    material.h \
    shell.h \
    elements/finitelement.h \
    elements/hexa.h \
    elements/lines.h \
    elements/quad.h \
    elements/tetra.h \
    elements/tria.h \
    eigenmode.h \
    eigenmodes.h \
    fem.h \
    bdfentity.h \
    femwidget.h
