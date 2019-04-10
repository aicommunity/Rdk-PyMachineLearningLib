#-------------------------------------------------
#
# Project created by QtCreator 2012-11-11T15:17:55
#
#-------------------------------------------------

QT       -= gui

TARGET = Rdk-PyMachineLearningLib.qt
TEMPLATE = lib
CONFIG += staticlib

#DEFINES += RDK_USE_CUDA
#DEFINES += RDK_USE_PYTHON

#unix {
#    DEFINES += RDK_USE_DARKNET
#}

include($$PWD/../../../../Rdk/Build/Lib/Qt/RdkDefines.pri)

    contains(DEFINES,RDK_USE_CUDA) {
        DEFINES += GPU
}

DEFINES += LIBRDK_LIBRARY_EXPORT
DEFINES += RDK_UNICODE_RUN
DEFINES += RDK_QT
DEFINES += BOOST_PYTHON_STATIC_LIB
DEFINES += BOOST_NUMPY_STATIC_LIB

VERSION = $$system(hg parents --template '{rev}')
DEFINES += RDK_LIB_CVS_REVISION=$$VERSION

INCLUDEPATH += $$PWD/../../../../Rdk/Deploy/Include

unix {
    CONFIG += c++11

    contains(DEFINES,RDK_USE_PYTHON) {
        INCLUDEPATH += $$(ANACONDA_PATH)/include/python3.6m/
        INCLUDEPATH += $$(ANACONDA_PATH)/lib/python3.6/site-packages/numpy/core/include/numpy/
}
    INCLUDEPATH += $$(OPENCV3_PATH)/include
    INCLUDEPATH += $$(BOOST_PATH)/include

    contains(DEFINES, GPU) {
        INCLUDEPATH += /usr/local/cuda-9.0/include
}

    DESTDIR = $$PWD/../../../../Bin/Platform/Linux/Lib.Qt

    contains(DEFINES,RDK_USE_DARKNET) {

    HEADERS += ../../ThirdParty/darknet/include/darknet.h \
    ../../Core/TDarknetObjectDetector.h \
    ../../Core/TDarknetUBitmapClassifier.h \
    ../../ThirdParty/darknet/include/darknet_utils.h

    SOURCES += ../../ThirdParty/darknet/include/darknet_utils.cpp \
    ../../Core/TDarknetUBitmapClassifier.cpp \
    ../../Core/TDarknetObjectDetector.cpp
}
} else:windows {
    DESTDIR = $$PWD/../../../../Bin/Platform/Win/Lib.Qt
    contains(DEFINES,RDK_USE_PYTHON) {
        INCLUDEPATH += $$(ANACONDA_PATH)/include/
        INCLUDEPATH += $$(ANACONDA_PATH)/Lib/site-packages/numpy/core/include/numpy
}
    INCLUDEPATH += $$(BOOST_PATH)
    INCLUDEPATH += $$(OPENCV3_PATH)/build/include

    contains(DEFINES, GPU) {
        INCLUDEPATH += $$(CUDA_PATH)/include
}
}

contains(DEFINES,RDK_USE_PYTHON) {

HEADERS += \
    ../../Core/pyboostcvconverter.hpp \
    ../../Core/TPythonIntegrationUtil.h \
    ../../Core/TPyAggregateClassifier.h \
    ../../Core/TPyUBitmapClassifier.h \
    ../../Core/TPythonIntegrationInclude.h \
    ../../Core/TPyObjectDetector.h \
    ../../Core/TPyObjectDetectorBasic.h \
    ../../Core/Lib.h \
    ../../Core/TPyComponent.h


SOURCES += \
    ../../Core/pyboost_cv3_converter.cpp \
    ../../Core/TPythonIntegrationUtil.cpp \
    ../../Core/TPyAggregateClassifier.cpp \
    ../../Core/TPyUBitmapClassifier.cpp \
    ../../Core/TPyObjectDetector.cpp \
    ../../Core/TPyObjectDetectorBasic.cpp \
    ../../Core/Lib.cpp \
    ../../Core/TPyComponent.cpp
}

HEADERS += \
    ../../Core/Lib.h


SOURCES += \
    ../../Core/Lib.cpp
