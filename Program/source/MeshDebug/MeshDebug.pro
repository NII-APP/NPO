
TEMPLATE = app

TARGET = xyu

QT       += opengl widgets core gui


SOURCES += \
	main.cpp \
    ../NPO/engine/afr.cpp \
    ../NPO/engine/afrarray.cpp \
    ../Mesh/bdfentity.cpp \
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
    ../Mesh/elements/tria.cpp


include (../CGL/sharedCGL.pri)

HEADERS += \
    ../NPO/engine/afr.h \
    ../NPO/engine/afrarray.h \
    ../Mesh/bdfentity.h \
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
    ../Mesh/elements/tria.h

RESOURCES += \
    ../NPO/res.qrc