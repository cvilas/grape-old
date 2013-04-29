# get versions from this file
include(../version)

# library description
TARGET = GrapeUtils
VERSION = $${UTILS_VERSION}

# common settings
include(../grapelibs.pri)

# library specific settings
win32:DEFINES += GRAPEUTILS_DLL GRAPEUTILS_DLL_EXPORT
#win32:DEFINES -= _UNICODE UNICODE
INCLUDEPATH += ./
HEADERS = consoleio.h stringfuncs.h CmdLineArgs.h grapeutils_common.h
SOURCES = consoleio.cpp CmdLineArgs.cpp

win32:HEADERS += guidfuncs.h
win32:SOURCES += guidfuncs.cpp
unix:SOURCES += 

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lGrapeCored0 -lRpcrt4
    else:unix: LIBS += -lGrapeCored
} else {
    win32:LIBS += -lGrapeCore0 -lRpcrt4
    else:unix: LIBS += -lGrapeCore
}

