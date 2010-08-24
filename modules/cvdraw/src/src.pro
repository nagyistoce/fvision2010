QT -= core \
    gui

TEMPLATE = lib
CONFIG += staticlib
TARGET = cvdraw

include("../using_opencv.pri")

DESTDIR = ../lib

INCLUDEPATH += ../include/fvision/cvdraw \
    .

INCLUDEPATH += ../../cvutils/include

SOURCES += \
    TextDrawer.cpp \
    PointPairsDrawer.cpp \
    PointDrawer.cpp \
    PointCorrespondencesDrawer.cpp \
    LineDrawer.cpp \
    IndexedPointDrawer.cpp \
    ImageDemoHelper.cpp \
    EpipolarLinesDrawer.cpp \
    drawing.cpp \
    DisparitiesDrawer.cpp

HEADERS += \
    ../include/fvision/cvdraw/TextDrawer.h \
    ../include/fvision/cvdraw/PointPairsDrawer.h \
    ../include/fvision/cvdraw/PointDrawer.h \
    ../include/fvision/cvdraw/PointCorrespondencesDrawer.h \
    ../include/fvision/cvdraw/LineDrawer.h \
    ../include/fvision/cvdraw/IndexedPointDrawer.h \
    ../include/fvision/cvdraw/ImageDemoHelper.h \
    ../include/fvision/cvdraw/EpipolarLinesDrawer.h \
    ../include/fvision/cvdraw/drawing.h \
    ../include/fvision/cvdraw/DisparitiesDrawer.h \
    ../include/fvision/cvdraw.h
