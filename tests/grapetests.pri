QT       += testlib
QT       -= gui
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = $${PWD}/../bin
DLLDESTDIR = $${PWD}/../bin

win32:DEFINES += GRAPECORE_DLL GRAPEIO_DLL GRAPEUTILS_DLL GRAPETIMING_DLL
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/ -lGrapeIo0 -lGrapeTiming0 -lGrapeUtils0 -lGrapeCore0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/ -lGrapeIod0 -lGrapeTimingd0 -lGrapeUtilsd0 -lGrapeCored0
else:unix: LIBS += -L$$PWD/../lib/ -lGrapeIo -lGrapeTiming -lGrapeUtils -lGrapeCore -lpthread -lrt

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

