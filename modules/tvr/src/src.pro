QT -= core \
    gui

TEMPLATE = lib
CONFIG += staticlib
TARGET = tvr

include("../using_opencv.pri")

DESTDIR = ../lib

INCLUDEPATH += ../include/fvision/tvr \
    .

INCLUDEPATH += ../../cvutils/include \
	../../cvdraw/include \
	../../geom/include \
	../../calculators/include \
	../../moca/include

HEADERS += \
    TwoViewReconstructorImpl.h \
    TwoViewReconstructionDemo.h \
    FDecomposer.h \
    FDecomposeBasedCameraReconstructor.h \
    FComposer.h \
    CameraReconstructorImpl.h \
    ../include/fvision/tvr/TwoViewReconstructorFactory.h \
    ../include/fvision/tvr/TwoViewReconstructor.h \
    ../include/fvision/tvr/PointReconstructor.h \
    ../include/fvision/tvr/CameraReconstructorFactory.h \
    ../include/fvision/tvr/CameraReconstructor.h

SOURCES += \
    TwoViewReconstructorImpl.cpp \
    TwoViewReconstructorFactory.cpp \
    TwoViewReconstructor.cpp \
    TwoViewReconstructionDemo.cpp \
    PointReconstructor.cpp \
    FDecomposer.cpp \
    FDecomposeBasedCameraReconstructor.cpp \
    FComposer.cpp \
    CameraReconstructorImpl.cpp \
    CameraReconstructorFactory.cpp
