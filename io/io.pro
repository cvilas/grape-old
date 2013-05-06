# get versions from this file
include(../version)

# library description
TARGET = GrapeIo
VERSION = $${IO_VERSION}

# common settings
include(../grapelibs.pri)

# library specific settings
win32:DEFINES += GRAPECORE_DLL GRAPEUTILS_DLL GRAPEIO_DLL GRAPEIO_DLL_EXPORT
INCLUDEPATH += ./

HEADERS = IPort.h SerialPort.h \
    grapeio_common.h \
    IJoystick.h \
    SimpleJoystick.h
SOURCES = \
    IJoystick.cpp

win32:HEADERS += Dx8JoystickManager.h
win32:SOURCES += Dx8JoystickManager.cpp SimpleJoystick_windows.cpp SerialPort_windows.cpp
unix:SOURCES += SerialPort_unix.cpp SimpleJoystick_unix.cpp

CONFIG(debug, release|debug) {
    win32:LIBS += -lGrapeCored0 -lGrapeUtilsd0
    else:unix: LIBS += -lGrapeCored -lGrapeUtilsd
} else {
    win32:LIBS += -lGrapeCore0 -lGrapeUtils0
    else:unix: LIBS += -lGrapeCore -lGrapeUtilsd
}
