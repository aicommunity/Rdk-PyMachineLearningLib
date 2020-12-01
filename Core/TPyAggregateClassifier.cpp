#define NO_IMPORT_ARRAY

#ifndef RDK_TPyAggregateClassifierCPP
#define RDK_TPyAggregateClassifierCPP

#include "TPyAggregateClassifier.h"
#include <iostream>

namespace RDK {
/*
#if (PY_VERSION_HEX >= 0x03000000)
    void *init_py() {
#else
    void init_py(){
#endif
        if(Py_IsInitialized())
            return NUMPY_IMPORT_ARRAY_RETVAL;
        Py_Initialize();

        import_array();
        np::initialize();
        return NUMPY_IMPORT_ARRAY_RETVAL;
    }
*/
// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------  //DetectionClass("DetectionClass",this),
TPyAggregateClassifier::TPyAggregateClassifier(void)
: InputImage("InputImage",this),
  Detections("Detections",this),
  //DetectionReliability("DetectionReliability",this),
  DebugImage("DebugImage",this),
  AggrRectsMatrix("AggrRectsMatrix", this),
  AggrIdMatrix("AggrIdMatrix", this),
  PythonScriptFileName("PythonScriptFileName",this)
{
}

TPyAggregateClassifier::~TPyAggregateClassifier(void)
{
}
// --------------------------


// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
TPyAggregateClassifier* TPyAggregateClassifier::New(void)
{
 return new TPyAggregateClassifier;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
bool TPyAggregateClassifier::Initialize(void)
{
    PyGILState_STATE gil_state;
    Py_BLOCK_GIL
    try
    {
        LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Python init started..."));

        if(!PyEval_ThreadsInitialized())
        {
         LogMessageEx(RDK_EX_FATAL,__FUNCTION__,std::string("Python Py_Initialize didn't called!"));
         LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("Python init fail"));
         Py_UNBLOCK_GIL
         return false;
        }
//        init_py();
//        py::to_python_converter<cv::Mat, pbcvt::matToNDArrayBoostConverter>();
//        py::to_python_converter<RDK::UBitmap, pbcvt::uBitmapToNDArrayBoostConverter>();
        py::object MainModule = py::import("__main__");  // импортируем main-scope, см. https://docs.python.org/3/library/__main__.html
        py::object MainNamespace = MainModule.attr("__dict__");  // извлекаем область имен

        //py::object pycv2 = py::import("cv2");

        // TODO: путь для импорта файла брать из конфига"../../../../Libraries/Rdk-PyMachineLearningLib/PythonScripts/classifier_interface.py"
        // загрузка кода из файла в извлеченную область имен
        std::string s = this->GetEnvironment()->GetCurrentDataDir()+*PythonScriptFileName;
        py::object ClassifierInterfaceModule = import("test_class",s,MainNamespace);
        // экземпляр питоновского класса, через который активируется виртуальная среда и загружается модель
        // TODO: пусть до среды брать из конфига
        IntegrationInterface = ClassifierInterfaceModule.attr("ClassifierEmbeddingInterface");
        if(!IntegrationInterface.is_none())
            IntegrationInterfaceInstance = IntegrationInterface(); ///home/arnold/.virtualenvs/cv
        Py_UNBLOCK_GIL
        //boost::python::object rand_mod = boost::python::import("random");
        //boost::python::object rand_func = rand_mod.attr("random");
        //boost::python::object rand2 = rand_func();
        //std::cout << boost::python::extract<int>(rand2) << std::endl;

        std::cout << "Python init successs" << std::endl;
        Initialized = true;
    }
    catch (py::error_already_set const &)
    {
        Py_UNBLOCK_GIL
        Initialized = false;
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("Python init fail: ")+perrorStr);
    }
    catch(...)
    {
        Initialized = false;
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("Python init fail: Undandled exception"));
    }
 LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("...Python init finished successful!"));
 return true;
}

void TPyAggregateClassifier::AInit(void)
{
}

void TPyAggregateClassifier::AUnInit(void)
{
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPyAggregateClassifier::ADefault(void)
{
 Initialized=false;
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyAggregateClassifier::ABuild(void)
{
 if(IsInit())
  Initialize();
 return true;
}

// Сброс процесса счета без потери настроек
bool TPyAggregateClassifier::AReset(void)
{
 if(!Initialized)
  Initialize();
 return true;
}

// Выполняет расчет этого объекта
bool TPyAggregateClassifier::ACalculate(void)
{
 if(!Initialized)
  return true;

 if(!InputImage.IsConnected())
  return true;

 if (InputImage->GetColorModel() != RDK::ubmY8)
 {
     LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("Incorrect image color model. Need ubmY8 got: ")+sntoa(InputImage->GetColorModel()));
     return true;
 }

 DebugImage->SetColorModel(ubmRGB24,false);
 InputImage->ConvertTo(*DebugImage);

 Graph.SetCanvas(DebugImage);

 Detections->Resize(0, 6);

 if(AggrRectsMatrix->GetRows() != AggrIdMatrix->GetRows())
 {
  LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("Rows count not equals: AggrRectsMatrix(")+sntoa(AggrRectsMatrix->GetRows())+std::string(") != AggrIdMatrix(")+sntoa(AggrIdMatrix->GetRows())+")");
  return true;
 }

 Detections->Resize(AggrRectsMatrix->GetRows(), 6);

 RDK::UBitmap& input_img=*InputImage;
 //Пройти по всем агрегатам
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

  /// Тут считаем
  PyGILState_STATE gil_state;
  Py_BLOCK_GIL
  try
  {
   py::object retval = IntegrationInterfaceInstance.attr("classify")(obj_rect);
   Py_UNBLOCK_GIL

   object_cls = boost::python::extract<int>(retval);
  }
  catch (py::error_already_set const &)
  {
   Py_UNBLOCK_GIL
   std::string perrorStr = parse_python_exception();
   LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python error: ")+perrorStr);
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


 /// А теперь раскладываем результаты по выходам



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



