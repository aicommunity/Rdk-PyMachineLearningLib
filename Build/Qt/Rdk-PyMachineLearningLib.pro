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

    DESTDIR = $$PWD/../../../../Bin/Platform/Linux/Lib.Qt

} else:windows {
    DESTDIR = $$PWD/../../../../Bin/Platform/Win/Lib.Qt
}

contains(DEFINES,RDK_USE_PYTHON) {

HEADERS += \
    ../../Core/pyboostcvconverter.hpp \
    ../../Core/TPythonIntegrationUtil.h \
    ../../Core/TPyAggregateClassifier.h \
    ../../Core/TPyUBitmapClassifier.h \
    ../../Core/TPythonIntegrationInclude.h \
    ../../Core/TPyObjectDetector.h \
    ../../Core/TPySegmentator.h \
    ../../Core/TPyObjectDetectorYolo.h \
    ../../Core/TPyObjectDetectorYoloEx.h \
    ../../Core/TPyObjectDetectorSqueezeDet.h \
    ../../Core/TPySegmentatorProtobuf.h \
    ../../Core/TPySegmentatorUNet.h \
    ../../Core/Lib.h \
    ../../Core/TPyComponent.h   \
    ../../Core/TPyClassifierTrainer.h \
    ../../Core/TPyDetectorTrainer.h


SOURCES += \
    ../../Core/pyboost_cv3_converter.cpp \
    ../../Core/TPythonIntegrationUtil.cpp \
    ../../Core/TPyAggregateClassifier.cpp \
    ../../Core/TPyUBitmapClassifier.cpp \
    ../../Core/TPyObjectDetector.cpp \
    ../../Core/TPySegmentator.cpp \
    ../../Core/TPyObjectDetectorYolo.cpp \
    ../../Core/TPyObjectDetectorYoloEx.cpp \
    ../../Core/TPyObjectDetectorSqueezeDet.cpp \
    ../../Core/TPySegmentatorProtobuf.cpp \
    ../../Core/TPySegmentatorUNet.cpp \
    ../../Core/Lib.cpp \
    ../../Core/TPyComponent.cpp \
    ../../Core/TPyClassifierTrainer.cpp \
    ../../Core/TPyDetectorTrainer.cpp
}

HEADERS += \
    ../../Core/Lib.h


SOURCES += \
    ../../Core/Lib.cpp
