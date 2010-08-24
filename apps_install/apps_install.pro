TEMPLATE = lib
QMAKE_POST_LINK = make install
libs.files = $$PWD/../apps/point_tracking_tools/bin/*.exe

libs.path = $$PWD/../bin
INSTALLS +=libs
