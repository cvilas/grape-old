TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += core io \
    tests/TestIo

OTHER_FILES += \
    doc/doxygen/doxydoc.h \
    doc/doxygen/doxygen.cfg
