include(CGL/sharedCGL.pri)
include(Mesh/sharedMesh.pri)
include(../lib/igolib.pri)

INCLUDEPATH += $$PWD
contains(TARGET, "tests") {
} else {
    SOURCES += $$PWD/main.cpp
}

HEADERS += $$PWD/*.h \
    $$PWD/gui/viewerTab/viewertab.h \
    $$PWD/gui/viewerTab/viewerview.h \
    $$PWD/gui/toolbox/filenameedit.h \
    $$PWD/gui/toolbox/macdisplay.h \
    $$PWD/gui/toolbox/pixmapwidget.h \
    $$PWD/gui/toolbox/femviewer/femviewer.h \
    $$PWD/gui/pairsTab/pairstab.h \
    $$PWD/gui/pairsTab/pairsview.h \
    $$PWD/gui/toolbox/tablistwidget.h \
    $$PWD/gui/modesIdentificationWizard/modesidentchart.h \
    $$PWD/gui/modesIdentificationWizard/modesidentificationwizard.h \
    $$PWD/gui/mainWindow/maintabbar.h \
    $$PWD/gui/mainWindow/mainwindow.h \
    $$PWD/gui/toolbox/femviewer/fempairviewer.h \
    $$PWD/gui/pairWizard/truncationwizard.h \
    $$PWD/gui/pairWizard/relationdialog.h \
    $$PWD/gui/pairWizard/modelspicker.h \
    $$PWD/gui/toolbox/femviewer/popupmode.h \
    $$PWD/gui/toolbox/femviewer/toolbar.h \
    $$PWD/gui/toolbox/femviewer/modeinput.h \
    $$PWD/gui/toolbox/femviewer/magnitudeinput.h \
    $$PWD/gui/toolbox/femviewer/frequencyinput.h \
    $$PWD/gui/toolbox/femviewer/femscreen.h \
    $$PWD/gui/toolbox/femviewer/relationmodeinput.h \
    $$PWD/kernel/modelupdate.h \
    $$PWD/kernel/igofesolver.h \
    $$PWD/project/project.h \
    $$PWD/project/pairsmodel.h \
    $$PWD/project/femmodel.h \
    $$PWD/kernel/eigenmodesfounder.h \
    $$PWD/project.h

SOURCES += \
    $$PWD/gui/viewerTab/viewertab.cpp \
    $$PWD/gui/viewerTab/viewerview.cpp \
    $$PWD/gui/toolbox/filenameedit.cpp \
    $$PWD/gui/toolbox/macdisplay.cpp \
    $$PWD/gui/toolbox/pixmapwidget.cpp \
    $$PWD/gui/toolbox/femviewer/femviewer.cpp \
    $$PWD/gui/pairsTab/pairstab.cpp \
    $$PWD/gui/pairsTab/pairsview.cpp \
    $$PWD/gui/toolbox/tablistwidget.cpp \
    $$PWD/gui/modesIdentificationWizard/modesidentchart.cpp \
    $$PWD/gui/modesIdentificationWizard/modesidentificationwizard.cpp \
    $$PWD/gui/mainWindow/maintabbar.cpp \
    $$PWD/gui/mainWindow/mainwindow.cpp \
    $$PWD/gui/toolbox/femviewer/fempairviewer.cpp \
    $$PWD/gui/pairWizard/truncationwizard.cpp \
    $$PWD/gui/pairWizard/relationdialog.cpp \
    $$PWD/gui/pairWizard/modelspicker.cpp \
    $$PWD/gui/toolbox/femviewer/popupmode.cpp \
    $$PWD/gui/toolbox/femviewer/toolbar.cpp \
    $$PWD/gui/toolbox/femviewer/modeinput.cpp \
    $$PWD/gui/toolbox/femviewer/magnitudeinput.cpp \
    $$PWD/gui/toolbox/femviewer/frequencyinput.cpp \
    $$PWD/gui/toolbox/femviewer/femscreen.cpp \
    $$PWD/gui/toolbox/femviewer/relationmodeinput.cpp \
    $$PWD/kernel/modelupdate.cpp \
    $$PWD/identity.cpp \
    $$PWD/application.cpp \
    $$PWD/kernel/igofesolver.cpp \
    $$PWD/project/project.cpp \
    $$PWD/project/pairsmodel.cpp \
    $$PWD/project/femmodel.cpp \
    $$PWD/kernel/eigenmodesfounder.cpp

