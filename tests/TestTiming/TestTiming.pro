QT       += testlib
QT       -= gui

TARGET = TestTiming
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = $${PWD}/../../bin
DLLDESTDIR = $${PWD}/../../bin/

HEADERS += \
    TestStopWatch.h \
    TestTimer.h
SOURCES += TestTiming.cpp \
    TestStopWatch.cpp \
    TestTimer.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:DEFINES += GRAPECORE_DLL GRAPETIMING_DLL
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lGrapeTiming0 -lGrapeCore0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lGrapeTimingd0 -lGrapeCored0
else:unix: LIBS += -L$$PWD/../../lib/ -lGrapeTiming -lGrapeCore -lpthread -lrt

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../
