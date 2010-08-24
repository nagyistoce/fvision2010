QT -= core \
    gui

TEMPLATE = lib
CONFIG += staticlib
TARGET = langutils

DESTDIR = ../lib

INCLUDEPATH += ../include/fvision \
    .

SOURCES += \
    xgetopt.cpp \
    string_utils.cpp \
    random_utils.cpp \
    Observer.cpp \
    math_utils.cpp \
    format_utils.cpp \
    FileOutputHelper.cpp \
    file_utils_win.cpp \
    fassert.cpp

HEADERS += \
    ../include/fvision/langutils/type_utils.h \
    ../include/fvision/langutils/template_utils.h \
    ../include/fvision/langutils/string_utils.h \
    ../include/fvision/langutils/random_utils.h \
    ../include/fvision/langutils/Observer.h \
    ../include/fvision/langutils/mem_template.h \
    ../include/fvision/langutils/math_utils.h \
    ../include/fvision/langutils/iterator_template.h \
    ../include/fvision/langutils/io_template.h \
    ../include/fvision/langutils/IndexIterator.h \
    ../include/fvision/langutils/format_utils.h \
    ../include/fvision/langutils/FileOutputHelper.h \
    ../include/fvision/langutils/file_utils.h \
    ../include/fvision/langutils/fassert.h \
    ../include/fvision/langutils/xgetopt.h \
    ../include/fvision/langutils.h
