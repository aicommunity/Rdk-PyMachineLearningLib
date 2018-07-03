#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API
#ifndef RDK_TPyAggregateClassifierCPP
#define RDK_TPyAggregateClassifierCPP

#include "TPyAggregateClassifier.h"
#include "TPythonIntegrationUtil.h"
#include <iostream>
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python/numpy.hpp>
#include "pyboostcvconverter.hpp"
#include "boost/python/stl_iterator.hpp"

namespace py = boost::python;
namespace np = boost::python::numpy;

namespace RDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------  //DetectionClass("DetectionClass",this),
TPyAggregateClassifier::TPyAggregateClassifier(void)
: InputImage("InputImage",this),
  Detections("Detections",this),
  //DetectionReliability("DetectionReliability",this),
  DebugImage("DebugImage",this),
  AggrRectsMatrix("AggrRectsMatrix", this),
  AggrIdMatrix("AggrIdMatrix", this),
  InputFile("InputFile",this)
{
}

TPyAggregateClassifier::~TPyAggregateClassifier(void)
{
}
// --------------------------


// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
TPyAggregateClassifier* TPyAggregateClassifier::New(void)
{
 return new TPyAggregateClassifier;
}
// --------------------------

#if (PY_VERSION_HEX >= 0x03000000)

    static void *init_py() {
#else
        static void init_py(){
#endif
        Py_Initialize();

        import_array();
        np::initialize();
        return NUMPY_IMPORT_ARRAY_RETVAL;
    }

// --------------------------
// ������� ������ ���������� ������
// --------------------------
void TPyAggregateClassifier::AInit(void)
{
    /*Py_Initialize();
    bool res = np::initialize();
    try
    {
        py::object main_module = py::import("__main__");
        py::object main_namespace = main_module.attr("__dict__");

        py::object ignored = py::exec("hello = open('/home/ivan/hello.txt', 'w')\n"
                          "hello.write('Hello world!')\n"
                          "hello.close()",
                          main_namespace);
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = RDK::parse_python_exception();
        // TODO: ���������� � �������� ������ � ������������ ���������
        std::cout << "Error occured:" << std::endl << perrorStr << std::endl;
        std::cout << "Python init fail" << std::endl;
    }*/

    try
    {
        init_py();
        py::to_python_converter<cv::Mat, pbcvt::matToNDArrayBoostConverter>();
        py::to_python_converter<RDK::UBitmap, pbcvt::uBitmapToNDArrayBoostConverter>();
        py::object MainModule = py::import("__main__");  // ����������� main-scope, ��. https://docs.python.org/3/library/__main__.html
        py::object MainNamespace = MainModule.attr("__dict__");  // ��������� ������� ����

        //py::object pycv2 = py::import("cv2");

        // TODO: ���� ��� ������� ����� ����� �� �������"../../../../Libraries/Rdk-PyMachineLearningLib/PythonScripts/classifier_interface.py"
        // �������� ���� �� ����� � ����������� ������� ����
        std::string s = (*InputFile);
        py::object ClassifierInterfaceModule = import("test_class",s,MainNamespace);
        // ��������� ������������ ������, ����� ������� ������������ ����������� ����� � ����������� ������
        // TODO: ����� �� ����� ����� �� �������
        IntegrationInterface = ClassifierInterfaceModule.attr("ClassifierEmbeddingInterface");
        if(!IntegrationInterface.is_none())
            IntegrationInterfaceInstance = IntegrationInterface(); ///home/arnold/.virtualenvs/cv

        //boost::python::object rand_mod = boost::python::import("random");
        //boost::python::object rand_func = rand_mod.attr("random");
        //boost::python::object rand2 = rand_func();
        //std::cout << boost::python::extract<int>(rand2) << std::endl;

        std::cout << "Python init successs" << std::endl;
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        // TODO: ���������� � �������� ������ � ������������ ���������
        std::cout << "Error occured:" << std::endl << perrorStr << std::endl;
        std::cout << "Python init fail" << std::endl;
    }

}

void TPyAggregateClassifier::AUnInit(void)
{
}

// �������������� �������� �� ��������� � ����� �������� �����
bool TPyAggregateClassifier::ADefault(void)
{
 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TPyAggregateClassifier::ABuild(void)
{
 return true;
}

// ����� �������� ����� ��� ������ ��������
bool TPyAggregateClassifier::AReset(void)
{
 return true;
}

// ��������� ������ ����� �������
bool TPyAggregateClassifier::ACalculate(void)
{
 if(!InputImage.IsConnected())
  return true;

 DebugImage->SetColorModel(ubmRGB24,false);
 InputImage->ConvertTo(*DebugImage);

 Graph.SetCanvas(DebugImage);

 Detections->Resize(AggrRectsMatrix->GetRows(), 6);

 RDK::UBitmap& input_img=*InputImage;
 //������ �� ���� ���������
 for(int i = 0; i < AggrRectsMatrix->GetRows(); ++i)
 {
  (*Detections)(i, 0) = (*AggrIdMatrix)(i, 0);
  (*Detections)(i, 1) = (*AggrRectsMatrix)(i, 0);
  (*Detections)(i, 2) = (*AggrRectsMatrix)(i, 1);
  (*Detections)(i, 3) = (*AggrRectsMatrix)(i, 2);
  (*Detections)(i, 4) = (*AggrRectsMatrix)(i, 3);
  (*Detections)(i, 5) = -1;
  int object_cls = -1;

  UBitmap obj_rect;
  int width = (*AggrRectsMatrix)(i, 2) - (*AggrRectsMatrix)(i, 0);
  int height = (*AggrRectsMatrix)(i, 3) - (*AggrRectsMatrix)(i, 1);

  if((width<=3)||(height<=3))
    continue;

  obj_rect.SetRes(width, height, ubmY8);
  input_img.CopyTo(0,0,(*AggrRectsMatrix)(i, 0), (*AggrRectsMatrix)(i, 1), width, height, obj_rect);

  /// ��� �������
  try
  {
   py::object retval = IntegrationInterfaceInstance.attr("classify")(obj_rect);

   object_cls = boost::python::extract<int>(retval);
  }
  catch (py::error_already_set const &)
  {
   std::string perrorStr = parse_python_exception();
   // TODO: ����������
   std::cout << "Error occured:" << std::endl << perrorStr << std::endl;
   std::stringstream ss;
   ss<<"Python ERROR:" << perrorStr;
   LogMessageEx(RDK_EX_INFO,__FUNCTION__,ss.str());
  }
  if(object_cls==0)
  {
   Graph.SetPenColor(0x0000FF);
  }
  else if(object_cls==1)
  {
   Graph.SetPenColor(0x00FF00);
  }
  if(object_cls!=-1)
  {
   Graph.Rect((*AggrRectsMatrix)(i, 0),(*AggrRectsMatrix)(i, 1),(*AggrRectsMatrix)(i, 2),(*AggrRectsMatrix)(i, 3));
  }


  (*Detections)(i, 5) = object_cls;
 }
/*

 //LogMessageEx(RDK_EX_INFO,__FUNCTION__,"test");
 //MDMatrix<int> &detections=*Detections;
 //MDMatrix<int> &detection_class=*DetectionClass;
 //MDMatrix<double> &detection_reliability=*DetectionReliability;


 /// � ������ ������������ ���������� �� �������



 int num_objects(0);
 for(int i=0;i<num_objects;i++)
 {
  int x(0),y(0),width(0),height(0);
  int class_id(0);
  double reliability(0.0);

  detections.Resize(num_objects,4);
  detection_class.Resize(num_objects,1);
  detection_reliability.Resize(num_objects,1);

  detections(i,0)=x;
  detections(i,1)=y;
  detections(i,2)=width;
  detections(i,3)=height;
  detection_class(i,0)=class_id;
  detection_reliability(i,0)=reliability;

  Graph.SetPenColor(0x0000FF);
  Graph.Rect(x,y,x+width,y+height);
 }*/

 return true;
}
// --------------------------

}
#endif



