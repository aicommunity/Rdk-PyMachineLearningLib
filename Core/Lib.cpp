#ifndef RDK_UPyMachineLearningLib_CPP
#define RDK_UPyMachineLearningLib_CPP

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
#include <numpy/ndarrayobject.h>
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


#include "Lib.h"

#include "TPythonIntegrationInclude.h"

namespace RDK {

UPyMachineLearningLib PyMachineLearningLib;

// --------------------------
// ������������ � �����������
// --------------------------
UPyMachineLearningLib::UPyMachineLearningLib(void)
 : ULibrary("PyMachineLearningLib","1.0")
{
 try
 {
  init_py(); // �������� ������ �� �������� ������ ����������!!!
 }
 catch(...)
 {
  cout<<"PyMachineLearningLib: init_py() thors exception!"<<endl;
 }
}
// --------------------------

// --------------------------
// ������ ����������� �����������
// --------------------------
// ��������� ������ ClassSamples �������� ������������ �������� � �� �������.
// �� ��������� ��������������� ������� ������� � ������ ������.
void UPyMachineLearningLib::CreateClassSamples(UStorage *storage)
{
    py::to_python_converter<cv::Mat, pbcvt::matToNDArrayBoostConverter>();
    py::to_python_converter<RDK::UBitmap, pbcvt::uBitmapToNDArrayBoostConverter>();
 //UploadClass<TPythonIntegration>("TPythonIntegration", "PythonIntegration");

    /*UContainer *cont=0;

 cont=new TPythonIntegration;
 cont->Default();
 cont->SetName("PythonIntegration");
 UploadClass("TPythonIntegration",cont);*/

 UploadClass<TPyUBitmapClassifier>("TPyUBitmapClassifier","PyUBitmapClassifier");
 UploadClass<TPyAggregateClassifier>("TPyAggregateClassifier","PyAggregateClassifier");
 UploadClass<TPyObjectDetectorBasic>("TPyObjectDetectorBasic","PyObjectDetectorBasic");
 UploadClass<TPyObjectDetector>("TPyObjectDetector","PyObjectDetector");

#ifdef __GNUC__
 UploadClass<TDarknetObjectDetector>("TDarknetObjectDetector","DarknetObjectDetector");
#endif
}
// --------------------------

}

#endif


