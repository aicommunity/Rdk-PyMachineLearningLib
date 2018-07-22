#-------------------------------------------------
#
# Project created by QtCreator 2012-11-11T15:17:55
#
#-------------------------------------------------

QT       -= gui

TARGET = Rdk-PyMachineLearningLib.qt
TEMPLATE = lib
CONFIG += staticlib

DEFINES += LIBRDK_LIBRARY_EXPORT
DEFINES += RDK_UNICODE_RUN
DEFINES += RDK_QT
DEFINES += BOOST_PYTHON_STATIC_LIB
DEFINES += BOOST_NUMPY_STATIC_LIB

INCLUDEPATH += $$PWD/../../../../Rdk/Deploy/Include

unix {
    INCLUDEPATH += /usr/include/python3.5
    DESTDIR = $$PWD/../../../../Bin/Platform/Linux/Lib.Qt
}

windows {
    DESTDIR = $$PWD/../../../../Bin/Platform/Win/Lib.Qt
    INCLUDEPATH += $$(ANACONDA_PATH)/include/
    INCLUDEPATH += $$(BOOST_PATH)
    INCLUDEPATH += $$(OPENCV3_PATH)/build/include
    INCLUDEPATH += $$(ANACONDA_PATH)/Lib/site-packages/numpy/core/include/numpy
}

HEADERS += \
    ../../Core/pyboostcvconverter.hpp \
    ../../Core/TPythonIntegrationUtil.h \
    ../../Core/TPyAggregateClassifier.h \
    ../../Core/Lib.h

SOURCES += \
    ../../Core/pyboost_cv3_converter.cpp \
    ../../Core/TPythonIntegrationUtil.cpp \
    ../../Core/TPyAggregateClassifier.cpp \
    ../../Core/Lib.cpp
