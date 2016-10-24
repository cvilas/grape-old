# get versions from this file
include(../version)

# library description
TARGET = GrapeGraphics
VERSION = $${GRAPHICS_VERSION}

# common settings
include(../grapelibs.pri)
QT += opengl charts

# library specific settings
win32:DEFINES += COIN_DLL GRAPECORE_DLL GRAPEUTILS_DLL GRAPEALGO_DLL GRAPEGRAPHICS_DLL GRAPEGRAPHICS_DLL_EXPORT
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
    ImageAnnotationViewerP.h \
    Plot.h
android:HEADERS +=
else: HEADERS += \
    SoQtg.h \
    SoRenderArea.h \
    SoQtOffscreenRenderer.h \
    SoScreenAnnotationKit.h \
    SoScreen2DAnnotationKit.h \
    SoScreenTextAnnotation.h \
    SoQtRenderArea.h \
    SoQtGraphicsScene.h \
    SoSceneViewer.h

SOURCES = \
    Annotation.cpp \
    TextAnnotation.cpp \
    TimeAnnotation.cpp \
    CounterAnnotation.cpp \
    ImageAnnotation.cpp \
    CrosshairAnnotation.cpp \
    ImageAnnotator.cpp \
    ImageAnnotationViewer.cpp \
    ImageAnnotationViewerP.cpp \
    Plot.cpp
android:SOURCES +=
else: SOURCES += \
    SoQtg.cpp \
    SoRenderArea.cpp \
    SoQtOffscreenRenderer.cpp \
    SoScreenAnnotationKit.cpp \
    SoScreen2DAnnotationKit.cpp \
    SoScreenTextAnnotation.cpp \
    SoQtRenderArea.cpp \
    SoQtGraphicsScene.cpp \
    SoSceneViewer.cpp

FORMS = \
    forms/crosshair_annotation_edit.ui \
    forms/image_annotation_select.ui \
    forms/image_annotation_edit.ui \
    forms/text_annotation_attributes.ui \
    forms/text_annotation_entry.ui \
    forms/timestamp_annotation_format.ui \
    forms/counter_annotation_format.ui

RESOURCES = graphics.qrc

unix:INCLUDEPATH +=
else:win32:INCLUDEPATH += $(COINDIR)/include
unix:LIBS += -lCoin
else:win32:LIBS += -L$$(COINDIR)/lib

CONFIG(debug, release|debug) {
    win32:LIBS += -lGrapeCored0 -lGrapeUtilsd0 -lcoin3d
    else:unix: LIBS += -lGrapeCored -lGrapeUtilsd
} else {
    win32:LIBS += -lGrapeCore0 -lGrapeUtils0 -lcoin3
    else:unix: LIBS += -lGrapeCore -lGrapeUtilsd
}

