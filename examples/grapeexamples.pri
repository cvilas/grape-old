TEMPLATE = app

# config settings
CONFIG += debug_and_release build_all resources thread
android:CONFIG += static
win32: CONFIG += embed_manifest_exe

CONFIG += console qt
QT += core gui widgets opengl

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

DESTDIR = $${PWD}/../bin
DLLDESTDIR = $${PWD}/../bin

LIBS += -L$$PWD/../lib/
win32:LIBS += -L$$(COINDIR)/lib -lUser32
win32:DEFINES += COIN_DLL GRAPECORE_DLL GRAPEIO_DLL GRAPEUTILS_DLL GRAPETIMING_DLL GRAPEGRAPHICS_DLL UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS
win32:CONFIG(debug, debug|release):DEFINES += _DEBUG
win32:CONFIG(release, debug|release):LIBS+= -lGrapeGraphics0 -lGrapeIo0 -lGrapeTiming0 -lGrapeUtils0 -lGrapeCore0 -lCoin
else:win32:CONFIG(debug, debug|release):LIBS+= -lGrapeGraphicsd0 -lGrapeIod0 -lGrapeTimingd0 -lGrapeUtilsd0 -lGrapeCored0 -lCoin
else: LIBS += -lGrapeGraphics -lGrapeIo -lGrapeTiming -lGrapeUtils -lGrapeCore
unix:!android: LIBS += -lCoin -lpthread -lrt

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

unix:!android: INCLUDEPATH += $$system(coin-config --includedir)
else:win32:INCLUDEPATH += $(COINDIR)/include
unix:!android: LIBS += $$system(coin-config --ldflags) $$system(coin-config --libs)
