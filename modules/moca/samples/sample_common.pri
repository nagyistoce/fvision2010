QT -= core \
    gui

CONFIG += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ../../bin
INCLUDEPATH += ../../include

INCLUDEPATH += ../../../cvutils/include
LIBS += -L../../../cvutils/lib -lcvutils


include("$$PWD/../using_opencv.pri")
