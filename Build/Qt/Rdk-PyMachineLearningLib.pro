#-------------------------------------------------
#
# Project created by QtCreator 2012-11-11T15:17:55
#
#-------------------------------------------------

QT       -= gui

TARGET = Rdk-PyMachineLearningLib.qt
TEMPLATE = lib
CONFIG += staticlib

DEFINES += RDK_USE_CUDA
DEFINES += RDK_USE_PYTHON

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
    DEFINES += GPU
    INCLUDEPATH += /usr/include/python3.5
    INCLUDEPATH += /usr/lib/python3/dist-packages/numpy/core/include/numpy/
    INCLUDEPATH += /usr/local/include
    INCLUDEPATH += /usr/local/cuda-9.0/include
    INCLUDEPATH += /usr/local/boost_1_68_0
    DESTDIR = $$PWD/../../../../Bin/Platform/Linux/Lib.Qt

    HEADERS += ../../ThirdParty/darknet/include/darknet.h \
    ../../Core/TDarknetObjectDetector.h \
    ../../Core/TDarknetUBitmapClassifier.h \
    ../../ThirdParty/darknet/include/darknet_utils.h

    SOURCES += ../../ThirdParty/darknet/include/darknet_utils.cpp \
    ../../Core/TDarknetUBitmapClassifier.cpp \
    ../../Core/TDarknetObjectDetector.cpp
} else:windows {
    DESTDIR = $$PWD/../../../../Bin/Platform/Win/Lib.Qt
    INCLUDEPATH += $$(ANACONDA_PATH)/include/
    INCLUDEPATH += $$(BOOST_PATH)
    INCLUDEPATH += $$(OPENCV3_PATH)/build/include
    INCLUDEPATH += $$(ANACONDA_PATH)/Lib/site-packages/numpy/core/include/numpy

    @
    contains(DEFINES, GPU) {
    INCLUDEPATH += $$(CUDA_PATH)/include
}
}

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
