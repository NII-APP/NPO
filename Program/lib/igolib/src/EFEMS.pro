#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T14:34:27
#
#-------------------------------------------------

QT       -= core gui

TARGET = EFEMS
TEMPLATE = lib

CONFIG += staticlib

DEFINES += EFEMS_LIBRARY

SOURCES += *.cpp

HEADERS += *.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
