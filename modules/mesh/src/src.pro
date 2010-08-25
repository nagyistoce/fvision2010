QT -= core \
    gui

TEMPLATE = lib
CONFIG += staticlib
TARGET = mesh

include("../using_opencv.pri")

DESTDIR = ../lib

INCLUDEPATH += ../include/fvision/mesh \
    .

INCLUDEPATH += ../../cvutils/include \
    ../../geom/include

HEADERS += \
    VRMLExporter.h \
    OffExporter.h \
    Delaunay.h \
    ../include/fvision/mesh.h \
    ../include/fvision/mesh/SurfaceMesh.h \
    ../include/fvision/mesh/output.h \
    ../include/fvision/mesh/MeshCreator.h \
    ../include/fvision/mesh/mesh_utils.h

SOURCES += \
    VRMLExporter.cpp \
    SurfaceMesh.cpp \
    output.cpp \
    OffExporter.cpp \
    MeshCreator.cpp \
    mesh_utils.cpp \
    Delaunay.cpp
