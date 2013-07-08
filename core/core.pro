# get versions from this file
include(../version)

# library description
TARGET = GrapeCore
VERSION = $${CORE_VERSION}

# common settings
include(../grapelibs.pri)

# library specific settings
win32:DEFINES += GRAPECORE_DLL GRAPECORE_DLL_EXPORT
INCLUDEPATH += ./
HEADERS += \ 
    Exception.h \
    grapecore_common.h
SOURCES += \
    Exception.cpp
win32:SOURCES +=
unix:SOURCES +=
win32:LIBS +=
