TEMPLATE = lib

OPENCV_DLL_DIR = $$PWD/../modules/opencv_1.0/bin
DEST_BIN_DIR = $$PWD/../bin

apps.path = $$DEST_BIN_DIR
#apps.files += ../apps/point_tracking_tools/bin/*.exe
#apps.files += ../apps/model_calculation_tools/bin/*.exe
#don't know why the above lines don't work
apps.files += ../apps/point_tracking_tools/bin/pc.exe
apps.files += ../apps/point_tracking_tools/bin/pc_view.exe
apps.files += ../apps/point_tracking_tools/bin/ptdb_view.exe
apps.files += ../apps/point_tracking_tools/bin/klt_point_tracking.exe
apps.files += ../apps/model_calculation_tools/bin/F_Calculator.exe
apps.files += ../apps/model_calculation_tools/bin/H_Calculator.exe
apps.files += $$OPENCV_DLL_DIR/*.dll
INSTALLS += apps
#QMAKE_POST_LINK += mkdir $$DEST_BIN_DIR &
#QMAKE_POST_LINK += cp ../apps/point_tracking_tools/bin/*.exe $$DEST_BIN_DIR &
#QMAKE_POST_LINK += cp ../apps/model_calculation_tools/bin/*.exe $$DEST_BIN_DIR &
#QMAKE_POST_LINK += cp $$OPENCV_DLL_DIR/*.dll $$DEST_BIN_DIR
