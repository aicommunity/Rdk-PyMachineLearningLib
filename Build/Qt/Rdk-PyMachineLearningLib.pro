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
INCLUDEPATH += $$PWD/../../../../Rdk/Deploy/Include
INCLUDEPATH += /usr/include/python3.5

unix {
    DESTDIR = $$PWD/../../../../Bin/Platform/Linux/Lib.Qt
}

windows {
    DESTDIR = $$PWD/../../../../Bin/Platform/Win/Lib.Qt
}

HEADERS += \
    ../../Core/pyboostcvconverter.hpp \
    ../../Core/TPythonIntegrationUtil.h \
    ../../Core/TPythonIntegration.h \
    ../../Core/Lib.h

SOURCES += \
    ../../Core/pyboost_cv3_converter.cpp \
    ../../Core/TPythonIntegrationUtil.cpp \
    ../../Core/TPythonIntegration.cpp \
    ../../Core/Lib.cpp
