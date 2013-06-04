
TARGET = AnnotationViewerExample
include(../grapeexamples.pri)

HEADERS += \

SOURCES += \
    AnnotationViewerExample.cpp

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}
