#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T17:42:41
#
#-------------------------------------------------

QT       += core opengl widgets gui

TARGET = NPO
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    geometry.cpp \
    animationoptions.cpp \
    CGL/cparse.cpp \
    CGL/cparallelepiped.cpp \
    CGL/clabel.cpp \
    CGL/cinterval.cpp \
    CGL/cglwidget.cpp \
    CGL/cgl.cpp \
    CGL/cexpression.cpp \
    CGL/cdimension.cpp \
    CGL/cchartdata.cpp \
    CGL/cchart3d.cpp \
    CGL/cchart.cpp \
    CGL/cglwidgetmenu.cpp \
    geometryform.cpp \
    form.cpp \
    CGL/cvertexes.cpp \
    CGL/cmatrix.cpp \
    core/tetra.cpp \
    core/tria.cpp \
    core/finitelement.cpp \
    core/lines.cpp \
    core/quad.cpp \
    CGL/crange.cpp \
    CGL/carray.cpp \
    geometrywidget.cpp \
    core/hexa.cpp \
    core/shell.cpp \
    pseudoInverse.cpp

HEADERS += \
    geometry.h \
    animationoptions.h \
    CGL/cparse.h \
    CGL/cparallelepiped.h \
    CGL/clabel.h \
    CGL/cinterval.h \
    CGL/cglwidget.h \
    CGL/cgl.h \
    CGL/cexpression.h \
    CGL/cdimension.h \
    CGL/cchartdata.h \
    CGL/cchart3d.h \
    CGL/cchart.h \
    CGL/cglwidgetmenu.h \
    geometryform.h \
    form.h \
    CGL/cvertexes.h \
    CGL/cmatrix.h \
    core/tetra.h \
    core/tria.h \
    core/finitelement.h \
    core/lines.h \
    core/quad.h \
    CGL/crange.h \
    CGL/carray.h \
    geometrywidget.h \
    core/hexa.h \
    core/shell.h \
    pseudoInverse.h

OTHER_FILES += \
    summator.vert

RESOURCES += \
    res.qrc
