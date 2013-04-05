# get versions from this file
include(../version)

# library description
TARGET = GrapeCore
VERSION = $${CORE_VERSION}

# common settings
include(../grapelibs.pri)

# library specific settings
win32:DEFINES -= _UNICODE UNICODE
INCLUDEPATH += ./
HEADERS += grape_common.h 
SOURCES += grape_common.cpp
win32:SOURCES +=
unix:SOURCES +=
win32:LIBS +=
