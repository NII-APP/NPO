INCLUDEPATH += $$PWD

CONFIG(debug, debug|release): {
OBJECTS_DIR = $$PWD/../build/debug/cgl/obj
LIBS += -L$$PWD/../build/binaries -lcgl_d
}
else: {
OBJECTS_DIR = $$PWD/../build/release/cgl/obj
LIBS += -L$$PWD/../build/binaries -lcgl
}
