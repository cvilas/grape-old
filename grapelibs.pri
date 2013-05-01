# Common settings for GRAPE project libraries

# For windows - location of windows SDK
WINSDKDIR=C:\PROGRA~2\WI3CF2~1\8.0
TEMPLATE = lib

# the following are used by individual libraries
VERSTR = '\\"$${VERSION}\\"'  # place quotes around the version string
DEFINES += VER=\"$${VERSTR}\" # create a VER macro containing the version string

# config settings
CONFIG += debug_and_release build_all resources thread
android:CONFIG += static
win32: CONFIG += dll embed_manifest_dll embed_manifest_exe

CONFIG += qt
QT += core
QT -= gui

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

# target directories
DESTDIR = $${PWD}/lib
DLLDESTDIR = $${PWD}/bin/

DEFINES +=
win32:DEFINES += UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS
#win32: DEFINES -= UNICODE _UNICODE
CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS +=
    else:unix: LIBS +=
} else {
    win32:LIBS +=
    else:unix: LIBS +=
}

# don't want linking against qtmain.lib
QMAKE_LIBS_QT_ENTRY=
INCLUDEPATH += $${PWD}
win32:INCLUDEPATH += $${WINSDKDIR}/Include/um
DEPENDPATH += ./
LIBS += -L$${PWD}/lib/
win32:LIBS += -L$${WINSDKDIR}/Lib/win8/um/x64 -ldxguid -ldinput8 -lrpcrt4 -lws2_32 -lUser32
