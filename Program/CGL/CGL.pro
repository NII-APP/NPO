#-------------------------------------------------
#
# Project created by QtCreator 2015-07-20T20:25:56
#
#-------------------------------------------------

QT       += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release): {
OBJECTS_DIR = $$PWD/../build/debug/cgl/obj
TARGET = cgl_d
}
else: {
OBJECTS_DIR = $$PWD/../build/release/cgl/obj
TARGET = cgl
}
DESTDIR = $$PWD/../build/binaries
RCC_DIR = $$PWD/../build/app/rcc
UI_DIR = $$PWD/../build/app/ui
MOC_DIR = $$PWD/../build/app/moc


SOURCES += [a-z]*.cpp

HEADERS += [a-z]*.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    CGL.pri
