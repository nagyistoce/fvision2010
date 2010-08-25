QT -= core \
    gui

CONFIG += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ../../bin

FVISION_DIR = $$PWD/../../..
INCLUDEPATH += $$FVISION_DIR/include
LIBS += -L$$FVISION_DIR/lib \
     -lcalculators -lgeom -llangutils -lpoint_tracking -lisio -lcvdraw -lcvutils -lcv -lcxcore -lhighgui



