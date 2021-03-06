# get versions from this file
include(../version)

# library description
TARGET = GrapeAlgorithms
VERSION = $${ALGO_VERSION}

# common settings
include(../grapelibs.pri)

# library specific settings
win32:DEFINES += GRAPECORE_DLL GRAPEUTILS_DLL GRAPEALGO_DLL GRAPEALGO_DLL_EXPORT
INCLUDEPATH += ./

HEADERS = \
    SlidingMin.h \
    SlidingMax.h \
    SlidingMean.h \
    RollingMean.hpp \
    RollingMean.h \
    SlidingMean.hpp
SOURCES =

CONFIG(debug, release|debug) {
    win32:LIBS += -lGrapeCored0 -lGrapeUtilsd0 -ldinput8
    else:unix: LIBS += -lGrapeCored -lGrapeUtilsd
} else {
    win32:LIBS += -lGrapeCore0 -lGrapeUtils0 -ldinput8
    else:unix: LIBS += -lGrapeCore -lGrapeUtilsd
}
