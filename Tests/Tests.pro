#-------------------------------------------------
#
# Project created by QtCreator 2015-12-09T22:24:03
#
#-------------------------------------------------


QT       += opengl widgets testlib\
            svg core gui\
            winextras

win32 {
    LIBS += -L$$PWD/../Program/lib/igolib
    INCLUDEPATH += $$PWD/../Program/lib/igolib
    DEPENDPATH += $$PWD/../Program/lib/igolib

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86_32 build")

        !win32-g++ {
            LIBS += -$$PWD/../Program/lib/igolib/lExtSolInterface
            PRE_TARGETDEPS += $$PWD/../Program/lib/igolib/ExtSolInterface.lib
        } else {
            LIBS += -$$PWD/../Program/lib/igolib/lEFEMS
            PRE_TARGETDEPS += $$PWD/../Program/lib/igolib/libEFEMS.a
        }
    } else {
        message("x86_64 build")

        LIBS += -$$PWD/../Program/lib/lExtSolInterface64
        PRE_TARGETDEPS += $$PWD/../Program/lib/igolib/ExtSolInterface64.lib
    }
} else {
    message("nix build doesn't allow winextras and Kiselev solver")
    DEFINES += "DISABLE_KISELEV_SOLVER"
}

TARGET = tests
TEMPLATE = app

include(../Program/source/source.pri)

INCLUDEPATH += $$PWD/../Program/NPO
INCLUDEPATH += $$PWD/../Program/Mesh
INCLUDEPATH += $$PWD/../Program/CGL

SOURCES += testafr.cpp \
    main.cpp

HEADERS += testafr.h


RESOURCES += \
    ../Program/res.qrc \
    testres.qrc


DISTFILES += \
    ../Program/resource/css.css \
    ../Program/resource/main.json
