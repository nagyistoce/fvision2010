QT -= core \
    gui

CONFIG += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ../../bin

FVISION_DIR = $$PWD/../../..
INCLUDEPATH += $$FVISION_DIR/include

MODULES_DIR = $$PWD/../../../modules

INCLUDEPATH += $$MODULES_DIR/geom/include
LIBS += -L$$MODULES_DIR/geom/lib -lgeom

INCLUDEPATH += $$MODULES_DIR/langutils/include
LIBS += -L$$MODULES_DIR/langutils/lib -llangutils

INCLUDEPATH += $$MODULES_DIR/memdb/include

INCLUDEPATH += $$MODULES_DIR/point_tracking/include
LIBS += -L$$MODULES_DIR/point_tracking/lib -lpoint_tracking

INCLUDEPATH += $$MODULES_DIR/isio/include
LIBS += -L$$MODULES_DIR/isio/lib -lisio

INCLUDEPATH += $$MODULES_DIR/cvdraw/include
LIBS += -L$$MODULES_DIR/cvdraw/lib -lcvdraw

INCLUDEPATH += $$MODULES_DIR/cvutils/include
LIBS += -L$$MODULES_DIR/cvutils/lib -lcvutils

OPENCV_DIR = $$MODULES_DIR/opencv_1.0

INCLUDEPATH += $$OPENCV_DIR/cv/include \
    $$OPENCV_DIR/cxcore/include \
    $$OPENCV_DIR/otherlibs/highgui

LIBS += -L$$OPENCV_DIR/lib -lcv -lcxcore -lhighgui


