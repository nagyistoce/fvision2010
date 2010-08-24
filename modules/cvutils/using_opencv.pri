OPENCV_DIR = $$PWD/../opencv_1.0

INCLUDEPATH += $$OPENCV_DIR/cv/include \
    $$OPENCV_DIR/cxcore/include \
    $$OPENCV_DIR/otherlibs/highgui

LIBS += -L$$OPENCV_DIR/lib -lcv -lcxcore -lhighgui

