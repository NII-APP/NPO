#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T17:42:41
# then we go hard
#
#-------------------------------------------------

QT       += core opengl gui svg
win32 {
    QT += winextras

    LIBS += -L$$PWD\..\igolib
    INCLUDEPATH += $$PWD\..\igolib
    DEPENDPATH += $$PWD\..\igolib

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")

        !win32-g++ {
            LIBS += -lExtSolInterface
            PRE_TARGETDEPS += $$PWD/../igolib/ExtSolInterface.lib
        } else {
            message("Kiselev lib doesn't compiled as *.a (gcc) lib")
            DEFINES += "DISABLE_KISELEV_SOLVER"
        }
    } else {
        message("x86_64 build")

        LIBS += -lExtSolInterface64
        PRE_TARGETDEPS += $$PWD/../igolib/ExtSolInterface64.lib
    }
} else {
    message("nix build doesn't allow winextras and Kiselev solver")
    DEFINES += "DISABLE_KISELEV_SOLVER"
}


RC_ICONS += "images/Bez_imeni-1.ico"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NPO

TEMPLATE = app

CONFIG += c++11

include(../CGL/sharedCGL.pri)
include(../Mesh/sharedMesh.pri)

SOURCES += main.cpp \
    relationdialog.cpp \
    application.cpp \
    identity.cpp \
    project.cpp \
    mainwindow.cpp \
    truncationwizard.cpp \
    maintabbar.cpp \
    viewertab.cpp \
    viewermodel.cpp \
    viewerview.cpp \
    femviewer.cpp \
    femviewerfrequencyinput.cpp \
    femviewermodeinput.cpp \
    femviewermagnitudeinput.cpp \
    viewernode.cpp \
    femprocessor.cpp \
    macdisplay.cpp \
    pairstab.cpp \
    pairsmodel.cpp \
    pairviewer.cpp \
    engine/fempair.cpp \
    pairsview.cpp \
    modesidentificationwizard.cpp \
    engine/afr.cpp \
    engine/afrarray.cpp \
    filenameedit.cpp \
    pixmapwidget.cpp \
    tablistwidget.cpp \
    modesidentchart.cpp

HEADERS += \
    relationdialog.h \
    application.h \
    identity.h \
    project.h \
    mainwindow.h \
    truncationwizard.h \
    maintabbar.h \
    viewertab.h \
    viewermodel.h \
    viewerview.h \
    femviewer.h \
    femviewerfrequencyinput.h \
    femviewermodeinput.h \
    femviewermagnitudeinput.h \
    viewernode.h \
    femprocessor.h \
    macdisplay.h \
    pairstab.h \
    pairsmodel.h \
    pairviewer.h \
    engine/fempair.h \
    pairsview.h \
    modesidentificationwizard.h \
    engine/afr.h \
    engine/afrarray.h \
    filenameedit.h \
    pixmapwidget.h \
    tablistwidget.h \
    modesidentchart.h

exists( engine/pyParse/BDFEntity.cpp ) {
    message( "Configuring with new parser" )
    SOURCES += engine/pyParse/BDFEntity.cpp
    HEADERS += engine/pyParse/BDFEntity.h
    DEFINES += "PyBDF"
}


RESOURCES += \
    res.qrc

DISTFILES += \
    css.css \
    main.json \
    readBDF.py \
    meshSummator.vert
