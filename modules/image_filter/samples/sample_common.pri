QT -= core \
    gui

CONFIG += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ../../bin
INCLUDEPATH += ../../include
LIBS += -L../../lib -limage_filter

include("$$PWD/../using_opencv.pri")
