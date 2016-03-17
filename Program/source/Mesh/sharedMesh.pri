INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/**.cpp
SOURCES += \
    $$PWD/elements/**.cpp
SOURCES += \
    $$PWD/afr/**.cpp

HEADERS += \
    $$PWD/**.h
HEADERS += \
    $$PWD/elements/**.h
HEADERS += \
    $$PWD/afr/**.h

DISTFILES += \
    $$PWD/meshSummator.vert

RESOURCES += \
    $$PWD/mesh.qrc
