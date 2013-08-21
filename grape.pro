TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += core utils io timing graphics tests examples
timing.depends = core
utils.depends = core
io.depends = utils core

OTHER_FILES += \
    license.txt \
    doc/doxygen/doxydoc.h \
    doc/doxygen/doxygen.cfg \
    etc/tasklist
