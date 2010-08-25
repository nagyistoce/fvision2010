TEMPLATE = lib
#QMAKE_POST_LINK = make install
#libs.files = ../apps/point_tracking_tools/bin/*.exe
#libs.path = $$PWD/../bin
#INSTALLS +=libs
QMAKE_POST_LINK += mkdir $$PWD/../bin &
QMAKE_POST_LINK += cp ../apps/point_tracking_tools/bin/*.exe $$PWD/../bin
