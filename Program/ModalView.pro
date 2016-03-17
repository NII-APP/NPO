#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T17:42:41
# then we go hard
#
#-------------------------------------------------

QT       += core opengl gui svg
win32 {
    QT += winextras

    LIBS += -L$$PWD\lib\igolib
    INCLUDEPATH += $$PWD\lib\igolib
    DEPENDPATH += $$PWD\lib\igolib

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86_32 build")

        !win32-g++ {
            LIBS += -lExtSolInterface
            PRE_TARGETDEPS += $$PWD/lib/igolib/ExtSolInterface.lib
        } else {
            LIBS += -lEFEMS
            PRE_TARGETDEPS += $$PWD/lib/igolib/libEFEMS.a
        }
    } else {
        message("x86_64 build")

        LIBS += -lExtSolInterface64
        PRE_TARGETDEPS += $$PWD/lib/igolib/ExtSolInterface64.lib
    }
} else {
    message("nix build doesn't allow winextras and Kiselev solver")
    DEFINES += "DISABLE_KISELEV_SOLVER"
}


RC_ICONS += "resource/images/Bez_imeni-1.ico"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModalView

TEMPLATE = app

CONFIG += c++11

include(source/source.pri)

exists( engine/pyParse/BDFEntity.cpp ) {
    message( "Configuring with new parser" )
    SOURCES += engine/pyParse/BDFEntity.cpp
    HEADERS += engine/pyParse/BDFEntity.h
    DEFINES += "PyBDF"
}


RESOURCES += \
    res.qrc

DISTFILES += \
    resource/css.css \
    resource/main.json \
#    readBDF.py \
