#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T17:42:41
# then we go hard
#
#-------------------------------------------------

QT       += core opengl gui svg winextras

RC_ICONS += "images/Bez_imeni-1.ico"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NPO

TEMPLATE = app

include(../CGL/CGL.pri)
include(../Mesh/Mesh.pri)

CONFIG(debug, debug|release): {
OBJECTS_DIR = $$PWD/../build/debug/main/obj
TARGET = main_d
}
else: {
OBJECTS_DIR = $$PWD/../build/release/main/obj
TARGET = main
}
DESTDIR = $$PWD/../build/binaries
RCC_DIR = $$PWD/../build/main/rcc
UI_DIR = $$PWD/../build/main/ui
MOC_DIR = $$PWD/../build/main/moc

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
    tablistwidget.cpp

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
    tablistwidget.h

exists( engine/pyParse/BDFEntity.cpp ) {
    message( "Configuring with new parser" )
    SOURCES += engine/pyParse/BDFEntity.cpp
    HEADERS += engine/pyParse/BDFEntity.h
    DEFINES += "PyBDF"
}

OTHER_FILES +=

RESOURCES += \
    res.qrc

DISTFILES += \
    css.css \
    main.json \
    readBDF.py \
    meshSummator.vert
