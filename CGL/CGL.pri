INCLUDEPATH += $$PWD

CONFIG(debug, debug|release): {
OBJECTS_DIR = $$PWD/../build/debug/lib/obj
LIBS += -L$$PWD/../binaries -llib_d
}
else: {
OBJECTS_DIR = $$PWD/../build/release/lib/obj
LIBS += -L$$PWD/../binaries -llib
}


