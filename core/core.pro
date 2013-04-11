# get versions from this file
include(../version)

# library description
TARGET = GrapeCore
VERSION = $${CORE_VERSION}

# common settings
include(../grapelibs.pri)

# library specific settings
win32:DEFINES += GRAPECORE_DLL GRAPECORE_DLL_EXPORT
win32:DEFINES -= _UNICODE UNICODE
INCLUDEPATH += ./
HEADERS += \ 
    stringfuncs.h \
    Status.h \
    grapecore_common.h
SOURCES += \
    Status.cpp \
    grapecore_common.cpp
win32:SOURCES +=
unix:SOURCES +=
win32:LIBS +=
