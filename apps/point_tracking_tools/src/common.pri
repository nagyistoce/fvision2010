QT -= core \
    gui

CONFIG += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ../../bin

FVISION_DIR = $$PWD/../../..
INCLUDEPATH += $$FVISION_DIR/include
LIBS += -L$$FVISION_DIR/lib \
    -lgeom -llangutils -lpoint_tracking -lisio -l cvdraw -lcvutils -lcv -lcxcore -lhighgui



