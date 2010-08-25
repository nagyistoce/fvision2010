QT -= core \
    gui

TEMPLATE = lib
CONFIG += staticlib
TARGET = geom

include("../using_opencv.pri")

DESTDIR = ../lib

INCLUDEPATH += ../include/fvision/geom \
    .

INCLUDEPATH += ../../cvutils/include

SOURCES += \
    twoviewgeom.cpp \
    singleviewgeom.cpp \
    rotation.cpp \
    rec3d_utils.cpp \
    pointpair.cpp \
    Line.cpp \
    distortion.cpp \
    CameraInternal.cpp \
    camera_utils.cpp \
    Camera.cpp \
    camera1d_utils.cpp \
    Camera1D.cpp

HEADERS += \
    ../include/fvision/geom.h \
    ../include/fvision/geom/twoviewgeom.h \
    ../include/fvision/geom/singleviewgeom.h \
    ../include/fvision/geom/rotation.h \
    ../include/fvision/geom/rec3d_utils.h \
    ../include/fvision/geom/projection_template.h \
    ../include/fvision/geom/pointpair.h \
    ../include/fvision/geom/point_transform_template.h \
    ../include/fvision/geom/Line.h \
    ../include/fvision/geom/distortion.h \
    ../include/fvision/geom/CameraInternal.h \
    ../include/fvision/geom/camera_utils.h \
    ../include/fvision/geom/Camera.h \
    ../include/fvision/geom/camera1d_utils.h \
    ../include/fvision/geom/Camera1D.h

