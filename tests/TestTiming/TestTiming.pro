include(../grapetests.pri)
TARGET = TestTiming

HEADERS += \
    TestStopWatch.h \
    TestTimer.h
SOURCES += TestTiming.cpp \
    TestStopWatch.cpp \
    TestTimer.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

