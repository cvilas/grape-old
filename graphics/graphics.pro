# get versions from this file
include(../version)

# library description
TARGET = GrapeGraphics
VERSION = $${GRAPHICS_VERSION}
QT += opengl

# common settings
include(../grapelibs.pri)

# library specific settings
win32:DEFINES += COIN_DLL GRAPECORE_DLL GRAPEUTILS_DLL GRAPEGRAPHICS_DLL GRAPEGRAPHICS_DLL_EXPORT
INCLUDEPATH += ./

HEADERS = \
SOURCES = \

CONFIG(debug, release|debug) {
    win32:LIBS += -lGrapeCored0 -lGrapeUtilsd0 -lCoin
    else:unix: LIBS += -lGrapeCored -lGrapeUtilsd -lCoin
} else {
    win32:LIBS += -lGrapeCore0 -lGrapeUtils0 -lCoin
    else:unix: LIBS += -lGrapeCore -lGrapeUtilsd -lCoin
}

