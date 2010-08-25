QT -= core \
    gui

TEMPLATE = lib
CONFIG += staticlib
TARGET = image_filter

include("../using_opencv.pri")

DESTDIR = ../lib

INCLUDEPATH += ../include/fvision \
    .

HEADERS += \
    ToGrayImageFilter.h \
    SmoothImageFilter.h \
    SmartImageFilterImpl.h \
    SequentialImageFilterImpl.h \
    ResizeImageFilter.h \
    ImageTypeInfo.h \
    ImageFilter.h \
    image_filter_utils.h \
    BinarizeImageFilter.h \
    ../include/fvision/image_filter.h

SOURCES += \
    ToGrayImageFilter.cpp \
    SmoothImageFilter.cpp \
    SmartImageFilterImpl.cpp \
    SequentialImageFilterImpl.cpp \
    ResizeImageFilter.cpp \
    ImageTypeInfo.cpp \
    ImageFilterFactory.cpp \
    image_filters.cpp \
    image_filter_utils.cpp \
    BinarizeImageFilter.cpp
