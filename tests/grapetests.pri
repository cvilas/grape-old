# Common settings for GRAPE tests

TEMPLATE = app

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
DESTDIR = ../../bin/
DLLDESTDIR = ../../bin/

DEFINES +=
win32: DEFINES += GRAPE_DLL _UNICODE _CRT_SECURE_NO_WARNINGS
CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lGrapeIod -lws2_32 -lUser32
    else:unix: LIBS += -lGrapeIod
} else {
    win32:LIBS += -lGrapeIo -lws2_32 -lUser32
    else:unix: LIBS += -lGrapeIo
}

# don't want linking against qtmain.lib
QMAKE_LIBS_QT_ENTRY=

INCLUDEPATH += ./ ../../core ../../io
win32:INCLUDEPATH +=

DEPENDPATH += ./ ../../core ../../io

LIBS += -L../../lib/
win32:LIBS +=

