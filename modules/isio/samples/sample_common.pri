QT -= core \
    gui

CONFIG += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ../../bin
INCLUDEPATH += ../../include
LIBS += -L../../lib -lisio

include("$$PWD/../using_opencv.pri")
