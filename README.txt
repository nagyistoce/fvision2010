Install
=======

1. Install Qt and QtCreator
2. In QtCreator, open fvision.pro
3. In QtCreator, click Projects->Build Settings->Build Steps->Add Build Step make, Make arguments: install
4. Build Project


TroubleShooting
===============

1. mingw32-make.exe¡± exited with code 255
see http://qt-project.org/forums/viewthread/6613
Try using a clean build environment (e.g. the ¡°Clear System Environment¡± option under Build Environment in the project settings in Qt Creator).
