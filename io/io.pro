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

HEADERS = SerialPort.h \
    grapeio_common.h \
    IJoystick.h \
    SimpleJoystick.h \
    SocketException.h \
    TcpSocket.h \
    UdpSocket.h \
    IpSocket.h \
    UdpServer.h \
    SerialPortException.h \
    IoException.h \
    IDataPort.h
SOURCES = \
    IJoystick.cpp \
    TcpSocket.cpp \
    UdpSocket.cpp \
    IpSocket.cpp \
    UdpServer.cpp

win32:HEADERS += Dx8JoystickManager.h
win32:SOURCES += Dx8JoystickManager.cpp SimpleJoystick_windows.cpp SerialPort_windows.cpp
unix:SOURCES += SerialPort_unix.cpp
unix:!android:SOURCES+=SimpleJoystick_unix.cpp

CONFIG(debug, release|debug) {
    win32:LIBS += -lGrapeCored0 -lGrapeUtilsd0 -ldinput8
    else:unix: LIBS += -lGrapeCored -lGrapeUtilsd
} else {
    win32:LIBS += -lGrapeCore0 -lGrapeUtils0 -ldinput8
    else:unix: LIBS += -lGrapeCore -lGrapeUtilsd
}
