#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API
#ifndef RDK_TPythonIntegrationCPP
#define RDK_TPythonIntegrationCPP

#include "TPythonIntegration.h"
#include "TPythonIntegrationUtil.h"
#include <iostream>
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>
#include <boost/numpy.hpp>
#include "pyboostcvconverter.hpp"

namespace py = boost::python;
namespace np = boost::numpy;

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPythonIntegration::TPythonIntegration(void)
: Param1("Param1",this),
  InputImage("InputImage",this),
  Detections("Detections",this),
  DetectionClass("DetectionClass",this),
  DetectionReliability("DetectionReliability",this),
  DebugImage("DebugImage",this)
{
}

TPythonIntegration::~TPythonIntegration(void)
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
TPythonIntegration* TPythonIntegration::New(void)
{
 return new TPythonIntegration;
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
// Скрытые методы управления счетом
// --------------------------
void TPythonIntegration::AInit(void)
{
    try
    {
        init_py();
        py::to_python_converter<cv::Mat, pbcvt::matToNDArrayBoostConverter>();
        py::to_python_converter<RDK::UBitmap, pbcvt::uBitmapToNDArrayBoostConverter>();
        py::object MainModule = py::import("__main__");  // импортируем main-scope, см. https://docs.python.org/3/library/__main__.html
        py::object MainNamespace = MainModule.attr("__dict__");  // извлекаем область имен

        // TODO: путь для импорта файла брать из конфига
        // загрузка кода из файла в извлеченную область имен
        py::object ClassifierInterfaceModule = RDK::import("classifier_interface",
         "/home/arnold/dev/rtc/nmsdk2/nmsdk2/Libraries/Rdk-PyMachineLearningLib/PythonScripts/classifier_interface.py",
         MainNamespace);
        // экземпляр питоновского класса, через который активируется виртуальная среда и загружается модель
        // TODO: пусть до среды брать из конфига
        IntegrationInterfaceInstance = ClassifierInterfaceModule.attr("ClassifierEmbeddingInterface")("/home/arnold/.virtualenvs/cv");

        std::cout << "Python init successs" << std::endl;
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = RDK::parse_python_exception();
        // TODO: логировать и выдавать ошибку с прекращением программы
        std::cout << "Error occured:" << std::endl << perrorStr << std::endl;
        std::cout << "Python init fail" << std::endl;
    }

}

void TPythonIntegration::AUnInit(void)
{
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPythonIntegration::ADefault(void)
{
 Param1=0;
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPythonIntegration::ABuild(void)
{
 return true;
}

// Сброс процесса счета без потери настроек
bool TPythonIntegration::AReset(void)
{
 return true;
}

// Выполняет расчет этого объекта
bool TPythonIntegration::ACalculate(void)
{
 if(!InputImage.IsConnected())
  return true;

 Graph.SetCanvas(&*DebugImage);
 *DebugImage=*InputImage;
 DebugImage->ReflectionX();
 LogMessageEx(RDK_EX_INFO,__FUNCTION__,"test");

 RDK::UBitmap& input_img=*InputImage;
 MDMatrix<int> &detections=*Detections;
 MDMatrix<int> &detection_class=*DetectionClass;
 MDMatrix<double> &detection_reliability=*DetectionReliability;

 /// Тут считаем

 /// А теперь раскладываем результаты по выходам
 try
 {
    IntegrationInterfaceInstance.attr("classify")(input_img);
 }
 catch (py::error_already_set const &)
 {
    std::string perrorStr = RDK::parse_python_exception();
    // TODO: логировать
    std::cout << "Error occured:" << std::endl << perrorStr << std::endl;
 }


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
 }

 return true;
}
// --------------------------

}
#endif



