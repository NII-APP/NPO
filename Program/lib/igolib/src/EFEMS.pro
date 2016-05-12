#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T14:34:27
#
#-------------------------------------------------

QT       -= core gui


win32 {
    !contains(QMAKE_TARGET.arch, x86_64) {
        !win32-g++ {
            message("32")
            TARGET = ../../../ExtSolInterface
        } else {
            message("32gnu")
            TARGET = ../../../EFEMS
        }
    } else {
        message("64")
        TARGET = ../../../ExtSolInterface64
    }
} else {
    message("nix build doesn't allow winextras and Kiselev solver")
    DEFINES += "DISABLE_KISELEV_SOLVER"
}

TEMPLATE = lib

CONFIG += staticlib

DEFINES += EFEMS_LIBRARY

SOURCES += *.cpp

HEADERS += *.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
