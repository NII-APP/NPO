#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T17:42:41
# then we go hard
#
#-------------------------------------------------

QT       += core opengl gui svg

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
    engine/geometrypair.cpp \
    engine/pseudoInverse.cpp \
    application.cpp \
    identity.cpp \
    project.cpp \
    mainwindow.cpp \
    truncationtab.cpp \
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
    fempairviewer.cpp \
    femprocessor.cpp \
    macdisplay.cpp

HEADERS += \
    relationdialog.h \
    engine/geometrypair.h \
    engine/pseudoInverse.h \
    application.h \
    identity.h \
    project.h \
    mainwindow.h \
    truncationtab.h \
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
    fempairviewer.h \
    femprocessor.h \
    macdisplay.h

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
