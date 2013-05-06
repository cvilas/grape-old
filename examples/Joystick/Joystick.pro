
TARGET = JoystickExamples

include(../grapeexamples.pri)

HEADERS += JoystickExamples.h
SOURCES += JoystickExamples.cpp SimpleJoystickExample.cpp
win32: SOURCES += Dx8JoystickManagerExample.cpp
