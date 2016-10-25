
TARGET = PlotExample
include(../grapeexamples.pri)

HEADERS += \
    PlotExample.h

SOURCES += \
    PlotExample.cpp

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}
