TEMPLATE = app

QT       += testlib
QT       -= gui
CONFIG   += console
CONFIG   -= app_bundle

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

DESTDIR = $${PWD}/../bin
DLLDESTDIR = $${PWD}/../bin

win32:DEFINES += GRAPECORE_DLL GRAPEIO_DLL GRAPEUTILS_DLL GRAPETIMING_DLL UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS
win32:CONFIG(debug, debug|release):DEFINES += _DEBUG
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/ -lGrapeIo0 -lGrapeTiming0 -lGrapeUtils0 -lGrapeCore0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/ -lGrapeIod0 -lGrapeTimingd0 -lGrapeUtilsd0 -lGrapeCored0
else: LIBS+= -L$$PWD/../lib/ -lGrapeIo -lGrapeTiming -lGrapeUtils -lGrapeCore
unix:!android: LIBS += -lpthread -lrt

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

