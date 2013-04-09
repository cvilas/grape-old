#-------------------------------------------------
#
# Project created by QtCreator 2013-04-09T11:58:40
#
#-------------------------------------------------

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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lGrapeCore0 -lGrapeIo0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lGrapeCored0 -lGrapeIod0
else:unix: LIBS += -L$$PWD/../../lib/ -lGrapeCore -lGrapeIo

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../

