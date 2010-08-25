TEMPLATE = lib
DEST_LIB_DIR = $$PWD/../lib
OPENCV_LIB_DIR = $$PWD/../modules/opencv_1.0/lib
QMAKE_POST_LINK += mkdir $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp ../modules/geom/lib/libgeom.a $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp ../modules/cvutils/lib/libcvutils.a $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp ../modules/cvdraw/lib/libcvdraw.a $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp ../modules/isio/lib/libisio.a $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp ../modules/point_tracking/lib/libpoint_tracking.a $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp ../modules/langutils/lib/liblangutils.a $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp ../modules/calculators/lib/libcalculators.a $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp ../modules/tvr/lib/libtvr.a $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp ../modules/mesh/lib/libmesh.a $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp $$OPENCV_LIB_DIR/cv.lib $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp $$OPENCV_LIB_DIR/cxcore.lib $$DEST_LIB_DIR &
QMAKE_POST_LINK += cp $$OPENCV_LIB_DIR/highgui.lib $$DEST_LIB_DIR

#QMAKE_POST_LINK = make install

#libs.files = ../modules/geom/lib/libgeom.a \
#    ../modules/cvutils/lib/libcvutils.a
#libs.path = DEST_LIB_DIR
#INSTALLS += libs
