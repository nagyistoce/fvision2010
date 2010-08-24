QT -= core \
    gui

TEMPLATE = lib
CONFIG += staticlib
TARGET = cvutils

include("../using_opencv.pri")

DESTDIR = ../lib

INCLUDEPATH += ../include/fvision/cvutils \
    .

SOURCES += \
    misc.cpp \
    iplimage_utils.cpp \
    io.cpp \
    geom.cpp \
    CvMatWrapper.cpp \
    cvmat_utils.cpp \
    Colors.cpp \
    point_arith.cpp

HEADERS += \
    ../include/fvision/cvutils/point_mat_conversion_template.h \
    ../include/fvision/cvutils/point_arith.h \
    ../include/fvision/cvutils/misc.h \
    ../include/fvision/cvutils/iplimage_utils.h \
    ../include/fvision/cvutils/io.h \
    ../include/fvision/cvutils/geom_template.h \
    ../include/fvision/cvutils/geom.h \
    ../include/fvision/cvutils/CvMatWrapper.h \
    ../include/fvision/cvutils/cvmat_utils.h \
    ../include/fvision/cvutils/Colors.h \
    ../include/fvision/cvutils.h
