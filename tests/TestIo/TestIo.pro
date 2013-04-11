QT       += testlib
QT       -= gui

TARGET = tst_testio_serialport
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = $${PWD}/../../bin
DLLDESTDIR = $${PWD}/../../bin/


SOURCES += tst_testio_serialport.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:DEFINES += GRAPECORE_DLL GRAPEIO_DLL
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lGrapeIo0 -lGrapeCore0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lGrapeIod0 -lGrapeCored0
else:unix: LIBS += -L$$PWD/../../lib/ -lGrapeIo -lGrapeCore

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../

