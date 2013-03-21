# Common settings for GRAPE project libraries

TEMPLATE = lib

# the following are used by individual libraries
VERSTR = '\\"$${VERSION}\\"'  # place quotes around the version string
DEFINES += VER=\"$${VERSTR}\" # create a VER macro containing the version string

# config settings
CONFIG += debug_and_release build_all resources thread
win32: CONFIG += dll embed_manifest_dll embed_manifest_exe

CONFIG += qt
QT += xml core
QT -= gui

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

# target directories
DESTDIR = ../lib/
DLLDESTDIR = ../bin/

DEFINES +=
win32: DEFINES += GRAPE_DLL GRAPE_DLL_EXPORT _UNICODE _CRT_SECURE_NO_WARNINGS
CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lws2_32 -lUser32
    else:unix: LIBS +=
} else {
    win32:LIBS += -lws2_32 -lUser32
    else:unix: LIBS +=
}

# don't want linking against qtmain.lib
QMAKE_LIBS_QT_ENTRY=
INCLUDEPATH += ./
win32:INCLUDEPATH += ../ThirdParty/lcm-w32-0-9-0/include \
                     ../ThirdParty/pthreads-w32-2-8-0/include


DEPENDPATH += ./

LIBS += -L../lib/ #-llcm
win32:LIBS += -L../ThirdParty/lcm-w32-0-9-0/lib \
              -L../ThirdParty/pthreads-w32-2-8-0/lib

