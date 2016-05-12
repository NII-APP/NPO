win32 {
    QT += winextras

    LIBS += -L$$PWD\igolib
    INCLUDEPATH += $$PWD\igolib
    DEPENDPATH += $$PWD\igolib

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86_32 build")

        !win32-g++ {
            LIBS += -lExtSolInterface
            PRE_TARGETDEPS += $$PWD/igolib/ExtSolInterface.lib
        } else {
            LIBS += -lEFEMS
            PRE_TARGETDEPS += $$PWD/igolib/libEFEMS.a
        }
    } else {
        message("x86_64 build")

        LIBS += -lExtSolInterface64
        PRE_TARGETDEPS += $$PWD/igolib/ExtSolInterface64.lib
    }
} else {
    message("nix build doesn't allow winextras and Kiselev solver")
    DEFINES += "DISABLE_KISELEV_SOLVER"
}
