# get versions from this file
include(../version)

# library description
TARGET = GrapeIo
VERSION = $${IO_VERSION}

# common settings
include(../grapelibs.pri)

# library specific settings
win32:DEFINES -= _UNICODE UNICODE
INCLUDEPATH += ./ ../core
HEADERS = IPort.h SerialPort.h
SOURCES =
win32:SOURCES += SerialPort_windows.cpp
unix:SOURCES += SerialPort_linux.cpp
win32:LIBS += ws2_32.lib
