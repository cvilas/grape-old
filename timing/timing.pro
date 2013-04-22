# get versions from this file
include(../version)

# library description
TARGET = GrapeTiming
VERSION = $${TIMING_VERSION}

# common settings
include(../grapelibs.pri)

# library specific settings
win32:DEFINES += GRAPECORE_DLL GRAPETIMING_DLL GRAPETIMING_DLL_EXPORT
win32:DEFINES -= _UNICODE UNICODE
INCLUDEPATH += ./
HEADERS += grapetiming_common.h StopWatch.h Timer.h
unix:HEADERS += posix.h
SOURCES +=
win32:SOURCES += StopWatch_windows.cpp Timer_windows2.cpp
unix:SOURCES += StopWatch_unix.cpp Timer_unix.cpp

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lGrapeCored0 -lwinmm
    else:unix: LIBS += -lGrapeCored
} else {
    win32:LIBS += -lGrapeCore0 -lwinmm
    else:unix: LIBS += -lGrapeCore
}

