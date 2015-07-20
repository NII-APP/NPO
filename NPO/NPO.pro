#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T17:42:41
# then we go hard
#
#-------------------------------------------------

QT       += core opengl gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NPO

TEMPLATE = app

SOURCES += main.cpp \
    engine/animationoptions.cpp \
    engine/geometrywidget.cpp \
    relationdialog.cpp \
    engine/form.cpp \
    engine/geometry.cpp \
    engine/geometryform.cpp \
    engine/geometrypair.cpp \
    engine/pseudoInverse.cpp \
    engine/core/finitelement.cpp \
    engine/core/hexa.cpp \
    engine/core/lines.cpp \
    engine/core/material.cpp \
    engine/core/quad.cpp \
    engine/core/shell.cpp \
    engine/core/tetra.cpp \
    engine/core/tria.cpp \
    application.cpp \
    identity.cpp \
    project.cpp \
    mainwindow.cpp \
    geometriesmodel.cpp \
    viewer.cpp \
    truncationtab.cpp \
    truncationwizard.cpp \
    maintabbar.cpp \
    engine/geometryobsoleteparse.cpp \
    engine/meshplace.cpp \
    libs/CGL/c2dchart.cpp \
    libs/CGL/c2dchartaxis.cpp \
    libs/CGL/c2dchartplace.cpp \
    libs/CGL/c3dchart.cpp \
    libs/CGL/c3dcolumnchart.cpp \
    libs/CGL/carray.cpp \
    libs/CGL/cchartdata.cpp \
    libs/CGL/ccolorscale.cpp \
    libs/CGL/cdimension.cpp \
    libs/CGL/cdimensionarray.cpp \
    libs/CGL/cdimensioninterval.cpp \
    libs/CGL/cexpression.cpp \
    libs/CGL/cgl.cpp \
    libs/CGL/cglwidget.cpp \
    libs/CGL/cinterval.cpp \
    libs/CGL/clabel.cpp \
    libs/CGL/cmatrix.cpp \
    libs/CGL/cparallelepiped.cpp \
    libs/CGL/cparse.cpp \
    libs/CGL/crange.cpp \
    libs/CGL/cscale.cpp \
    libs/CGL/cvertexes.cpp \
    libs/CGL/cylindercoordinatesystem.cpp \
    libs/CGL/rectangularcoordinatesystem.cpp

HEADERS += \
    engine/animationoptions.h \
    engine/geometrywidget.h \
    relationdialog.h \
    engine/form.h \
    engine/geometry.h \
    engine/geometryform.h \
    engine/geometrypair.h \
    engine/pseudoInverse.h \
    engine/core/finitelement.h \
    engine/core/hexa.h \
    engine/core/lines.h \
    engine/core/material.h \
    engine/core/quad.h \
    engine/core/shell.h \
    engine/core/tetra.h \
    engine/core/tria.h \
    application.h \
    identity.h \
    project.h \
    mainwindow.h \
    geometriesmodel.h \
    viewer.h \
    truncationtab.h \
    truncationwizard.h \
    maintabbar.h \
    engine/meshplace.h \
    libs/CGL/c2dchart.h \
    libs/CGL/c2dchartaxis.h \
    libs/CGL/c2dchartplace.h \
    libs/CGL/c3dchart.h \
    libs/CGL/c3dcolumnchart.h \
    libs/CGL/carray.h \
    libs/CGL/cchartdata.h \
    libs/CGL/ccolorscale.h \
    libs/CGL/cdimension.h \
    libs/CGL/cdimensionarray.h \
    libs/CGL/cdimensioninterval.h \
    libs/CGL/cexpression.h \
    libs/CGL/cgl.h \
    libs/CGL/cglwidget.h \
    libs/CGL/cinterval.h \
    libs/CGL/clabel.h \
    libs/CGL/cmatrix.h \
    libs/CGL/cparallelepiped.h \
    libs/CGL/cparse.h \
    libs/CGL/crange.h \
    libs/CGL/cscale.h \
    libs/CGL/cvertexes.h \
    libs/CGL/cylindercoordinatesystem.h \
    libs/CGL/rectangularcoordinatesystem.h


INCLUDEPATH += libs/CGL

exists( engine/pyParse/BDFEntity.cpp ) {
    message( "Configuring with new parser" )
    SOURCES += engine/pyParse/BDFEntity.cpp
    HEADERS += engine/pyParse/BDFEntity.h
    DEFINES += "PyBDF"
}

OTHER_FILES += \
    summator.vert

RESOURCES += \
    res.qrc

DISTFILES += \
    css.css \
    main.json
