# get versions from this file
include(../version)

# library description
TARGET = GrapeGraphics
VERSION = $${GRAPHICS_VERSION}

# common settings
include(../grapelibs.pri)
QT += opengl

# library specific settings
win32:DEFINES += COIN_DLL GRAPECORE_DLL GRAPEUTILS_DLL GRAPEGRAPHICS_DLL GRAPEGRAPHICS_DLL_EXPORT
INCLUDEPATH += ./

HEADERS = \
    grapegraphics_common.h \
    ISignalerField.h \
    Annotation.h \
    TextAnnotation.h \
    TimeAnnotation.h \
    CounterAnnotation.h \
    ImageAnnotation.h \
    CrosshairAnnotation.h \
    ImageAnnotator.h \
    ImageAnnotationViewer.h \
    ImageAnnotationViewerP.h

SOURCES = \
    Annotation.cpp \
    TextAnnotation.cpp \
    TimeAnnotation.cpp \
    CounterAnnotation.cpp \
    ImageAnnotation.cpp \
    CrosshairAnnotation.cpp \
    ImageAnnotator.cpp \
    ImageAnnotationViewer.cpp \
    ImageAnnotationViewerP.cpp

FORMS = \
    forms/crosshair_annotation_edit.ui \
    forms/image_annotation_select.ui \
    forms/image_annotation_edit.ui \
    forms/text_annotation_attributes.ui \
    forms/text_annotation_entry.ui \
    forms/timestamp_annotation_format.ui \
    forms/counter_annotation_format.ui

RESOURCES = graphics.qrc

INCLUDEPATH += $$system(coin-config --includedir)
LIBS += $$system(coin-config --ldflags) $$system(coin-config --libs)

CONFIG(debug, release|debug) {
    win32:LIBS += -lGrapeCored0 -lGrapeUtilsd0
    else:unix: LIBS += -lGrapeCored -lGrapeUtilsd
} else {
    win32:LIBS += -lGrapeCore0 -lGrapeUtils0
    else:unix: LIBS += -lGrapeCore -lGrapeUtilsd
}

