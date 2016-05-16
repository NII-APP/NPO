include(CGL/sharedCGL.pri)
include(Mesh/sharedMesh.pri)
include(../lib/igolib.pri)

INCLUDEPATH += $$PWD

HEADERS += $$PWD/*.h \
    $$PWD/gui/viewerTab/viewermodel.h \
    $$PWD/gui/viewerTab/viewertab.h \
    $$PWD/gui/viewerTab/viewerview.h \
    $$PWD/gui/toolbox/filenameedit.h \
    $$PWD/gui/toolbox/macdisplay.h \
    $$PWD/gui/toolbox/pixmapwidget.h \
    $$PWD/gui/toolbox/femviewer/femviewer.h \
    $$PWD/gui/toolbox/femviewer/femviewerfrequencyinput.h \
    $$PWD/gui/toolbox/femviewer/femviewermagnitudeinput.h \
    $$PWD/gui/toolbox/femviewer/femviewermodeinput.h \
    $$PWD/gui/pairsTab/pairsmodel.h \
    $$PWD/gui/pairsTab/pairstab.h \
    $$PWD/gui/pairsTab/pairsview.h \
    $$PWD/gui/pairsTab/pairviewer.h \
    $$PWD/gui/toolbox/tablistwidget.h \
    $$PWD/gui/modesIdentificationWizard/modesidentchart.h \
    $$PWD/gui/modesIdentificationWizard/modesidentificationwizard.h \
    $$PWD/gui/mainWindow/maintabbar.h \
    $$PWD/gui/mainWindow/mainwindow.h \
    $$PWD/gui/toolbox/femviewer/fempairviewer.h \
    $$PWD/gui/pairWizard/truncationwizard.h \
    $$PWD/gui/pairWizard/relationdialog.h \
    $$PWD/gui/pairWizard/modelspicker.h
SOURCES += $$PWD/*.cpp \
    $$PWD/gui/viewerTab/viewermodel.cpp \
    $$PWD/gui/viewerTab/viewertab.cpp \
    $$PWD/gui/viewerTab/viewerview.cpp \
    $$PWD/gui/toolbox/filenameedit.cpp \
    $$PWD/gui/toolbox/macdisplay.cpp \
    $$PWD/gui/toolbox/pixmapwidget.cpp \
    $$PWD/gui/toolbox/femviewer/femviewer.cpp \
    $$PWD/gui/toolbox/femviewer/femviewerfrequencyinput.cpp \
    $$PWD/gui/toolbox/femviewer/femviewermagnitudeinput.cpp \
    $$PWD/gui/toolbox/femviewer/femviewermodeinput.cpp \
    $$PWD/gui/pairsTab/pairsmodel.cpp \
    $$PWD/gui/pairsTab/pairstab.cpp \
    $$PWD/gui/pairsTab/pairsview.cpp \
    $$PWD/gui/pairsTab/pairviewer.cpp \
    $$PWD/gui/toolbox/tablistwidget.cpp \
    $$PWD/gui/modesIdentificationWizard/modesidentchart.cpp \
    $$PWD/gui/modesIdentificationWizard/modesidentificationwizard.cpp \
    $$PWD/gui/mainWindow/maintabbar.cpp \
    $$PWD/gui/mainWindow/mainwindow.cpp \
    $$PWD/gui/toolbox/femviewer/fempairviewer.cpp \
    $$PWD/gui/pairWizard/truncationwizard.cpp \
    $$PWD/gui/pairWizard/relationdialog.cpp \
    $$PWD/gui/pairWizard/modelspicker.cpp
