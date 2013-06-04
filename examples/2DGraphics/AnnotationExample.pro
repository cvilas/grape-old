
TARGET = AnnotationExample
include(../grapeexamples.pri)

HEADERS += \

SOURCES += \
    AnnotationExample.cpp 

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}
