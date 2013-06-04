
TARGET = MultiScreenExample
include(../grapeexamples.pri)

RESOURCES =
FORMS += ./forms/screen_number.ui
HEADERS +=

SOURCES += \
    MultiScreenExample.cpp

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}
