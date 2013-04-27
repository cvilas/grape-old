QT       -= gui
CONFIG   += console debug_and_release build_all resources
CONFIG   -= app_bundle
win32: CONFIG += dll embed_manifest_dll embed_manifest_exe console

TEMPLATE = app

DESTDIR = $${PWD}/../bin
DLLDESTDIR = $${PWD}/../bin

win32:DEFINES += GRAPECORE_DLL GRAPEIO_DLL GRAPEUTILS_DLL GRAPETIMING_DLL _CRT_SECURE_NO_WARNINGS
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/ -lGrapeIo0 -lGrapeTiming0 -lGrapeUtils0 -lGrapeCore0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/ -lGrapeIod0 -lGrapeTimingd0 -lGrapeUtilsd0 -lGrapeCored0
else:unix: LIBS += -L$$PWD/../lib/ -lGrapeIo -lGrapeTiming -lGrapeUtils -lGrapeCore -lpthread -lrt

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

