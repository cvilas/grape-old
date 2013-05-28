TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += core utils io timing tests examples
timing.depends = core
utils.depends = core
io.depends = utils core

OTHER_FILES += \
    doc/doxygen/doxydoc.h \
    doc/doxygen/doxygen.cfg \
    etc/tasklist
