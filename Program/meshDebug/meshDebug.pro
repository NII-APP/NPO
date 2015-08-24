#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T17:42:41
# then we go hard
#
#-------------------------------------------------

QT       += core opengl gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = meshDebug

TEMPLATE = app

include(../CGL/CGL.pri)
include(../Mesh/Mesh.pri)

CONFIG(debug, debug|release): {
OBJECTS_DIR = $$PWD/../build/debug/main/obj
TARGET = meshDebug_d
}
else: {
OBJECTS_DIR = $$PWD/../build/release/main/obj
TARGET = meshDebug
}
DESTDIR = $$PWD/../build/binaries
RCC_DIR = $$PWD/../build/main/rcc
UI_DIR = $$PWD/../build/main/ui
MOC_DIR = $$PWD/../build/main/moc

SOURCES += main.cpp \
    mainwindow.cpp

OTHER_FILES += \
    summator.vert \
    meshSummator.vert

RESOURCES += \
    res.qrc

HEADERS += \
    mainwindow.h
