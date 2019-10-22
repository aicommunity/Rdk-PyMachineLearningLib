#ifndef RDK_UPyMachineLearningLib_CPP
#define RDK_UPyMachineLearningLib_CPP

#ifdef RDK_USE_PYTHON

#ifndef PY_ARRAY_UNIQUE_SYMBOL
#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API
#endif

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#ifndef BOOST_PYTHON_STATIC_LIB
#define BOOST_PYTHON_STATIC_LIB
#endif
#include <boost/python.hpp>
#ifdef WIN32
#include <boost/python/numpy.hpp>
#include <boost/python/numpy/dtype.hpp>
#include <boost/python/numpy/ndarray.hpp>
#else
#include <boost/python/numpy.hpp>
#include <boost/python/numpy/dtype.hpp>
#include <boost/python/numpy/ndarray.hpp>
#endif
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>

#ifdef WIN32
#include <ndarrayobject.h>
#else
#include <ndarrayobject.h>
#endif

namespace py = boost::python;

#ifdef WIN32
namespace np = boost::python::numpy;
#else
namespace np = boost::python::numpy;
#endif

#if (PY_VERSION_HEX >= 0x03000000)
    extern void *init_py();
#else
    extern void init_py();
#endif

#if (PY_VERSION_HEX >= 0x03000000)
    void *init_py() {
#else
    void init_py(){
#endif
        if(Py_IsInitialized())
            return NUMPY_IMPORT_ARRAY_RETVAL;
        Py_Initialize();
//        if (!PyEval_ThreadsInitialized())
//        {
            PyEval_InitThreads();
//        }
        import_array();
        np::initialize();

        return NUMPY_IMPORT_ARRAY_RETVAL;
    }

#endif

#include "Lib.h"

#ifdef RDK_USE_PYTHON
#include "TPythonIntegrationInclude.h"
#endif

namespace RDK {

/// Флаг, сигнализирующий что питон-подсистема была инициализирована
bool UPyMachineLearningLib::PythonInitFlag(false);

UPyMachineLearningLib PyMachineLearningLib;

// --------------------------
// Конструкторы и деструкторы
// --------------------------
UPyMachineLearningLib::UPyMachineLearningLib(void)
 : ULibrary("PyMachineLearningLib","1.0", GetGlobalVersion())
{
 PythonInit();
}
// --------------------------

// --------------------------
// Методы заполенения бибилиотеки
// --------------------------
/// Однократная инициализация python-подистемы
void UPyMachineLearningLib::PythonInit(void)
{
#ifdef RDK_USE_PYTHON
 if(!PythonInitFlag)
 {
     try
     {
      init_py(); // Вызывать только из главного потока приложения!!!
     }
     catch(...)
     {
      cout<<"PyMachineLearningLib: init_py() throws exception!"<<endl;
     }
     py::to_python_converter<cv::Mat, pbcvt::matToNDArrayBoostConverter>();
     py::to_python_converter<RDK::UBitmap, pbcvt::uBitmapToNDArrayBoostConverter>();

  PythonInitFlag=true;
 }
#endif
}

// Заполняет массив ClassSamples готовыми экземплярами образцов и их именами.
// Не требуется предварительная очистка массива и уборка памяти.
void UPyMachineLearningLib::CreateClassSamples(UStorage *storage)
{
#ifdef RDK_USE_PYTHON
 UploadClass<TPyUBitmapClassifier>("TPyUBitmapClassifier","PyUBitmapClassifier");
 UploadClass<TPyAggregateClassifier>("TPyAggregateClassifier","PyAggregateClassifier");
 UploadClass<TPyObjectDetectorYoloEx>("TPyObjectDetectorBasic","PyObjectDetectorBasic");
 UploadClass<TPyObjectDetectorYolo>("TPyObjectDetector","PyObjectDetector");
 UploadClass<TPyObjectDetectorSqueezeDet>("TPyObjectDetectorSqueezeDet","PyObjectDetectorSqueezeDet");
 UploadClass<TPySegmentatorProtobuf>("TPySegmentatorProtobuf","PySegmentatorProtobuf");
 UploadClass<TPySegmentatorUNet>("TPySegmentatorUNet","TPySegmentatorUNet");
#endif

}
// --------------------------

}

#endif


