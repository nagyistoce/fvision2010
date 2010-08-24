TEMPLATE = lib
QMAKE_POST_LINK = make install
libs.files = $$PWD/../modules/isio/lib/libisio.a \
    $$PWD/../modules/cvutils/lib/libcvutils.a \
    $$PWD/../modules/cvdraw/lib/libcvdraw.a \
    $$PWD/../modules/geom/lib/libgeom.a \
    $$PWD/../modules/langutils/lib/liblangutils.a \
    $$PWD/../modules/point_tracking/lib/libpoint_tracking.a \
    $$PWD/../modules/opencv_1.0/lib/cv.lib \
    $$PWD/../modules/opencv_1.0/lib/cxcore.lib \
    $$PWD/../modules/opencv_1.0/lib/highgui.lib

libs.path = $$PWD/../lib
INSTALLS +=libs
