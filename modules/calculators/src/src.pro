QT -= core \
    gui

TEMPLATE = lib
CONFIG += staticlib
TARGET = calculators

include("../using_opencv.pri")

DESTDIR = ../lib

INCLUDEPATH += ../include/fvision/calculators \
    .

INCLUDEPATH += ../../cvutils/include \
	../../geom/include \
	../../moca/include

HEADERS += \
    template_utils.h \
    PointCalculatorImpl.h \
    LineSegmentIntersectionCalculatorImpl.h \
    HomographyCalculatorImpl.h \
    FundamentalMatrixCalculatorImpl.h \
    CameraCalculatorImpl.h \
    Camera1DCalculatorImpl.h \
    camera1d_calibration.h \
    ../include/fvision/calculators/TwoViewModelCalculator.h \
    ../include/fvision/calculators/PointCalculatorFactory.h \
    ../include/fvision/calculators/PointCalculator.h \
    ../include/fvision/calculators/LineSegmentIntersectionCalculatorFactory.h \
    ../include/fvision/calculators/LineSegmentIntersectionCalculator.h \
    ../include/fvision/calculators/HomographyCalculatorFactory.h \
    ../include/fvision/calculators/HomographyCalculator.h \
    ../include/fvision/calculators/FundamentalMatrixCalculatorFactory.h \
    ../include/fvision/calculators/FundamentalMatrixCalculator.h \
    ../include/fvision/calculators/CameraCalculatorFactory.h \
    ../include/fvision/calculators/CameraCalculator.h \
    ../include/fvision/calculators/camera_calibration.h \
    ../include/fvision/calculators/Camera1DCalculatorFactory.h \
    ../include/fvision/calculators/Camera1DCalculator.h \
    ../include/fvision/calculators.h

SOURCES += \
    TwoViewModelCalculator.cpp \
    PointCalculatorImpl.cpp \
    PointCalculatorFactory.cpp \
    PointCalculator.cpp \
    LineSegmentIntersectionCalculatorImpl.cpp \
    LineSegmentIntersectionCalculatorFactory.cpp \
    LineSegmentIntersectionCalculator.cpp \
    HomographyCalculatorImpl.cpp \
    HomographyCalculatorFactory.cpp \
    HomographyCalculator.cpp \
    FundamentalMatrixCalculatorImpl.cpp \
    FundamentalMatrixCalculatorFactory.cpp \
    FundamentalMatrixCalculator.cpp \
    CameraCalculatorImpl.cpp \
    CameraCalculatorFactory.cpp \
    CameraCalculator.cpp \
    camera_calibration.cpp \
    Camera1DCalculatorImpl.cpp \
    Camera1DCalculatorFactory.cpp \
    Camera1DCalculator.cpp \
    camera1d_calibration.cpp
