INCLUDEPATH += $$PWD

CONFIG(debug, debug|release): {
OBJECTS_DIR = $$PWD/../build/debug/mesh/obj
LIBS += -L$$PWD/../build/binaries -lmesh_d
}
else: {
OBJECTS_DIR = $$PWD/../build/release/mesh/obj
LIBS += -L$$PWD/../build/binaries -lmesh
}
