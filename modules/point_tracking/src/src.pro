QT -= core \
    gui

TEMPLATE = lib
CONFIG += staticlib
TARGET = point_tracking

include("../using_opencv.pri")

DESTDIR = ../lib

INCLUDEPATH += ../include/fvision/point_tracking \
    .

INCLUDEPATH += ../../memdb/include

HEADERS += \
    matching_utils.h \
    sift/sift_wrapper.h \
    sift/sift.h \
    sift/minpq.h \
    sift/KdTreeWrapper.h \
    sift/kdtree.h \
    sift/imgfeatures.h \
    impl/SiftTwoViewPointTracker.h \
    impl/SiftPointTracker.h \
    impl/PyrLKTwoViewPointTracker.h \
    impl/PyrLKPointTracker.h \
    impl/PointMatcher.h \
    impl/CornerPointMatchTwoViewPointTracker.h \
    impl/CornerDetector.h \
    impl/point_matchers/SimpleWindowPointMatcher.h \
    impl/point_matchers/PyrLKPointMatcher.h \
    impl/point_matchers/OpticalFlowPointMatcher.h \
    impl/corner_detectors/OpenCVCornerDetector.h \
    impl/corner_detectors/MultiScaleOpenCVCornerDetector.h \
    impl/corner_detectors/GridCornerDetector.h \
    impl/corner_detectors/BlockCornerDetector.h \
    sift/sift_utils.h

SOURCES += \
    TwoViewPointTrackerFactory.cpp \
    TwoViewPointTracker.cpp \
    PointTrackerFactory.cpp \
    PointTracker.cpp \
    matching_utils.cpp \
    sift/sift_wrapper.cpp \
    sift/sift.cpp \
    sift/minpq.cpp \
    sift/KdTreeWrapper.cpp \
    sift/kdtree.cpp \
    sift/imgfeatures.cpp \
    impl/SiftTwoViewPointTracker.cpp \
    impl/SiftPointTracker.cpp \
    impl/PyrLKTwoViewPointTracker.cpp \
    impl/PyrLKPointTracker.cpp \
    impl/CornerPointMatchTwoViewPointTracker.cpp \
    impl/CornerDetector.cpp \
    impl/point_matchers/SimpleWindowPointMatcher.cpp \
    impl/point_matchers/PyrLKPointMatcher.cpp \
    impl/point_matchers/OpticalFlowPointMatcher.cpp \
    impl/corner_detectors/OpenCVCornerDetector.cpp \
    impl/corner_detectors/MultiScaleOpenCVCornerDetector.cpp \
    impl/corner_detectors/GridCornerDetector.cpp \
    impl/corner_detectors/BlockCornerDetector.cpp \
    sift/sift_utils.cpp

OTHER_FILES += \
    sift/THANKS.txt \
    sift/README.txt \
    sift/LICENSE.ubc.txt \
    sift/LICENSE.txt \
    sift/ChangeLog.txt
