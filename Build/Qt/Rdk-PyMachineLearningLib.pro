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

unix {
INCLUDEPATH += /usr/include/python3.5m
INCLUDEPATH += /usr/local/include/boost
INCLUDEPATH += /usr/local/lib64
INCLUDEPATH += /usr/include/numpy
INCLUDEPATH += /usr/local/include/opencv
}


unix {
    DESTDIR = $$PWD/../../../../Bin/Platform/Linux/Lib.Qt
}

windows {
    DESTDIR = $$PWD/../../../../Bin/Platform/Win/Lib.Qt
}

#including opencv
OPENCV_LIBS_LIST = -lopencv_core \
 -lopencv_highgui \
 -lopencv_imgproc \
 -lopencv_videoio \
 -lopencv_video \
 -lopencv_imgcodecs

windows {
 OPENCV_LIBS_VERSION = 310

 # ??????? ????????? ????????(?????? ????????) ?? ???? ????????? ??????? ???????? ?????????
 defineReplace(addPostfix) {
  libList = $$1
  for(lib, libList) {
   returnValue += $${lib}$${2}
  }
  return($$returnValue)
 }

 INCLUDEPATH += $$(OPENCV3_PATH)/build/include

 CONFIG(debug){
  LIBS += -L$$(OPENCV3_PATH)/build/x86/Vc10cuda/lib $$addPostfix($$OPENCV_LIBS_LIST, $${OPENCV_LIBS_VERSION}d)
 }
 CONFIG(release){
  LIBS += -L$$(OPENCV3_PATH)/build/x86/Vc10cuda/lib $$addPostfix($$OPENCV_LIBS_LIST, $${OPENCV_LIBS_VERSION})
 }

} else:unix {
 LIBS += $$OPENCV_LIBS_LIST
}

HEADERS += \
    ../../Core/Lib.h \
    ../../Core/pyboostcvconverter.hpp \
    ../../Core/TPythonIntegration.h \
    ../../Core/TPythonIntegrationUtil.h

SOURCES += \
    ../../Core/Lib.cpp \
    ../../Core/pyboost_cv3_converter.cpp \
    ../../Core/TPythonIntegration.cpp \
    ../../Core/TPythonIntegrationUtil.cpp
