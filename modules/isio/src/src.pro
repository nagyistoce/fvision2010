QT -= core \
    gui

TEMPLATE = lib
CONFIG += staticlib
TARGET = isio

include("../using_opencv.pri")

DESTDIR = ../lib

INCLUDEPATH += ../include/fvision \
    .

HEADERS += \
    PathRegexImageSequenceWriter.h \
    PathRegexImageSequenceReader.h \
    file_utils.h \
    AviImageSequenceWriter.h \
    AviImageSequenceReader.h \
    ../include/fvision/isio.h

SOURCES += \
    PathRegexImageSequenceWriter.cpp \
    PathRegexImageSequenceReader.cpp \
    ImageSequenceWriterFactory.cpp \
    ImageSequenceReaderFactory.cpp \
    file_utils_win.cpp \
    AviImageSequenceWriter.cpp \
    AviImageSequenceReader.cpp
