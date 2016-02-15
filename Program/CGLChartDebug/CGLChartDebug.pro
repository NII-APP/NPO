QT       += core opengl gui svg winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CGLChartDebug

TEMPLATE = app

SUBDIRS += \
    ../CGL/CGL.pro \
    ../Mesh/Mesh.pro \
    ../CGL/CGL.pro

INCLUDEPATH += ../CGL

DISTFILES += \
    ../Mesh/Mesh.pri \
    ../CGL/CGL.pri

HEADERS += \
    ../CGL/c2dchart.h \
    ../CGL/c2dchartaxis.h \
    ../CGL/c2dchartplace.h \
    ../CGL/c3dchart.h \
    ../CGL/c3dcolumnchart.h \
    ../CGL/carray.h \
    ../CGL/cchartdata.h \
    ../CGL/ccolorscale.h \
    ../CGL/cdimension.h \
    ../CGL/cdimensionarray.h \
    ../CGL/cdimensioninterval.h \
    ../CGL/cexpression.h \
    ../CGL/cgl.h \
    ../CGL/cglwidget.h \
    ../CGL/cinterval.h \
    ../CGL/clabel.h \
    ../CGL/cmatrix.h \
    ../CGL/cparallelepiped.h \
    ../CGL/cparse.h \
    ../CGL/crange.h \
    ../CGL/cscale.h \
    ../CGL/cvertexes.h \
    ../CGL/cylindercoordinatesystem.h \
    ../CGL/rectangularcoordinatesystem.h \
    ../NPO/engine/afr.h \
    ../NPO/engine/afrarray.h \
    ../CGL/cchartdatalist.h \
    ../CGL/cslider.h \
    ../Mesh/bdfentity.h \
    ../Mesh/bdfwriter.h \
    ../Mesh/eigenmode.h \
    ../Mesh/eigenmodes.h \
    ../Mesh/fem.h \
    ../Mesh/femwidget.h \
    ../Mesh/material.h \
    ../Mesh/shell.h \
    ../Mesh/elements/finitelement.h \
    ../Mesh/elements/hexa.h \
    ../Mesh/elements/lines.h \
    ../Mesh/elements/quad.h \
    ../Mesh/elements/tetra.h \
    ../Mesh/elements/tria.h \
    ../CGL/cvector.h \
    ../CGL/csize.h

SOURCES += \
    ../CGL/c2dchart.cpp \
    ../CGL/c2dchartaxis.cpp \
    ../CGL/c2dchartplace.cpp \
    ../CGL/c3dchart.cpp \
    ../CGL/c3dcolumnchart.cpp \
    ../CGL/carray.cpp \
    ../CGL/cchartdata.cpp \
    ../CGL/ccolorscale.cpp \
    ../CGL/cdimension.cpp \
    ../CGL/cdimensionarray.cpp \
    ../CGL/cdimensioninterval.cpp \
    ../CGL/cexpression.cpp \
    ../CGL/cgl.cpp \
    ../CGL/cglwidget.cpp \
    ../CGL/cinterval.cpp \
    ../CGL/clabel.cpp \
    ../CGL/cmatrix.cpp \
    ../CGL/cparallelepiped.cpp \
    ../CGL/cparse.cpp \
    ../CGL/crange.cpp \
    ../CGL/cscale.cpp \
    ../CGL/cvertexes.cpp \
    ../CGL/cylindercoordinatesystem.cpp \
    ../CGL/rectangularcoordinatesystem.cpp \
    main.cpp \
    ../NPO/engine/afr.cpp \
    ../NPO/engine/afrarray.cpp \
    ../CGL/cchartdatalist.cpp \
    ../CGL/cslider.cpp \
    ../Mesh/bdfentity.cpp \
    ../Mesh/bdfwriter.cpp \
    ../Mesh/eigenmode.cpp \
    ../Mesh/eigenmodes.cpp \
    ../Mesh/fem.cpp \
    ../Mesh/fembdfparser.cpp \
    ../Mesh/femwidget.cpp \
    ../Mesh/material.cpp \
    ../Mesh/shell.cpp \
    ../Mesh/elements/finitelement.cpp \
    ../Mesh/elements/hexa.cpp \
    ../Mesh/elements/lines.cpp \
    ../Mesh/elements/quad.cpp \
    ../Mesh/elements/tetra.cpp \
    ../Mesh/elements/tria.cpp \
    ../CGL/cvector.cpp \
    ../CGL/csize.cpp
