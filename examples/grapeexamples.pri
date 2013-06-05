TEMPLATE = app

# config settings
CONFIG += debug_and_release build_all resources thread
android:CONFIG += static
win32: CONFIG += embed_manifest_exe

CONFIG += console qt
QT += core gui

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

DESTDIR = $${PWD}/../bin
DLLDESTDIR = $${PWD}/../bin

LIBS += -L$$PWD/../lib/
win32:LIBS += -lUser32
win32:DEFINES += GRAPECORE_DLL GRAPEIO_DLL GRAPEUTILS_DLL GRAPETIMING_DLL GRAPEGRAPHICS_DLL UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS
win32:CONFIG(debug, debug|release):DEFINES += _DEBUG
win32:CONFIG(release, debug|release):LIBS+= -lGrapeGraphics0 -lGrapeIo0 -lGrapeTiming0 -lGrapeUtils0 -lGrapeCore0
else:win32:CONFIG(debug, debug|release):LIBS+= -lGrapeGraphicsd0 -lGrapeIod0 -lGrapeTimingd0 -lGrapeUtilsd0 -lGrapeCored0
else:unix: LIBS += -lGrapeGraphics -lGrapeIo -lGrapeTiming -lGrapeUtils -lGrapeCore -lpthread -lrt

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

unix:INCLUDEPATH += $$system(coin-config --includedir)
unix:LIBS += $$system(coin-config --ldflags) $$system(coin-config --libs)
