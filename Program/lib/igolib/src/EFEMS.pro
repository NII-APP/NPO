#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T14:34:27
#
#-------------------------------------------------

QT       -= core gui


win32 {
    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86_32 build")

        !win32-g++ {
            TARGET = ../../../ExtSolInterface
        } else {
            TARGET = ../../../libEFEMS.a
        }
    } else {
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
