# get versions from this file
include(../version)

# library description
TARGET = GrapeIo
VERSION = $${IO_VERSION}

# common settings
include(../grapelibs.pri)

# library specific settings
win32:DEFINES += GRAPECORE_DLL GRAPEIO_DLL GRAPEIO_DLL_EXPORT
win32:DEFINES -= _UNICODE UNICODE
INCLUDEPATH += ./
HEADERS = IPort.h SerialPort.h \
    grapeio_common.h
SOURCES =
win32:SOURCES += SerialPort_windows.cpp
unix:SOURCES += SerialPort_linux.cpp

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lGrapeCored0
    else:unix: LIBS += -lGrapeCored
} else {
    win32:LIBS += -lGrapeCore0
    else:unix: LIBS += -lGrapeCore
}

