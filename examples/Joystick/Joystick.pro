TEMPLATE = subdirs
CONFIG += ordered
unix:!android:SUBDIRS += SimpleJoystickExample.pro
win32:SUBDIRS += Dx8JoystickManagerExample.pro

