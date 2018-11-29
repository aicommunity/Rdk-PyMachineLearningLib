#define NO_IMPORT_ARRAY

#ifndef RDK_TPyUBitmapClassifierCPP
#define RDK_TPyUBitmapClassifierCPP

#include "TPyUBitmapClassifier.h"
#include <iostream>

namespace RDK {
/*
#if (PY_VERSION_HEX >= 0x03000000)
    void *init_py_pyubclsfr() {
#else
    void init_py_pyubclsfr(){
#endif
        if(Py_IsInitialized())
            return NUMPY_IMPORT_ARRAY_RETVAL;
        Py_Initialize();

        import_array();
        np::initialize();
        return NUMPY_IMPORT_ARRAY_RETVAL;
    }*/

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------  //DetectionClass("DetectionClass",this),
TPyUBitmapClassifier::TPyUBitmapClassifier(void)
: InputImages("InputImages",this),
  Initialized(false),
  OutputClasses("OutputClasses",this),
  ImageColorModel("ImageColorModel",this),
  NumClasses("NumClasses",this),
  OutputConfidences("OutputConfidences", this)
  //PythonScriptFileName("PythonScriptFileName",this)
{
    AddLookupProperty("PythonScriptPath",ptPubParameter, new UVProperty<std::string,TPyUBitmapClassifier>(this,
                 &TPyUBitmapClassifier::SetPythonClassifierScriptPath,&TPyUBitmapClassifier::GetPythonClassifierScriptPath));

}

bool TPyUBitmapClassifier::SetPythonClassifierScriptPath(const std::string& path)
{
    PythonScriptFileName = path;
    Initialized=false;
    return true;
}
const std::string & TPyUBitmapClassifier::GetPythonClassifierScriptPath(void) const
{
    return PythonScriptFileName;
}

TPyUBitmapClassifier::~TPyUBitmapClassifier(void)
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
TPyUBitmapClassifier* TPyUBitmapClassifier::New(void)
{
 return new TPyUBitmapClassifier;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
void TPyUBitmapClassifier::AInit(void)
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
        // TODO: логировать и выдавать ошибку с прекращением программы
        std::cout << "Error occured:" << std::endl << perrorStr << std::endl;
        std::cout << "Python init fail" << std::endl;
    }*/

    try
    {
        LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Python init started..."));
//        init_py();
        py::to_python_converter<cv::Mat, pbcvt::matToNDArrayBoostConverter>();
        py::to_python_converter<RDK::UBitmap, pbcvt::uBitmapToNDArrayBoostConverter>();
        py::object MainModule = py::import("__main__");  // импортируем main-scope, см. https://docs.python.org/3/library/__main__.html
        py::object MainNamespace = MainModule.attr("__dict__");  // извлекаем область имен

        //py::object pycv2 = py::import("cv2");

        // TODO: путь для импорта файла брать из конфига"../../../../Libraries/Rdk-PyMachineLearningLib/PythonScripts/classifier_interface.py"
        // загрузка кода из файла в извлеченную область имен
        std::string s = this->GetEnvironment()->GetCurrentDataDir()+PythonScriptFileName;
        py::object ClassifierInterfaceModule = import("classifier_interface",s,MainNamespace);
        // экземпляр питоновского класса, через который активируется виртуальная среда и загружается модель
        // TODO: пусть до среды брать из конфига
        IntegrationInterface = ClassifierInterfaceModule.attr("ClassifierEmbeddingInterface");
        if(!IntegrationInterface.is_none())
            IntegrationInterfaceInstance = IntegrationInterface(); ///home/arnold/.virtualenvs/cv

        //boost::python::object rand_mod = boost::python::import("random");
        //boost::python::object rand_func = rand_mod.attr("random");
        //boost::python::object rand2 = rand_func();
        //std::cout << boost::python::extract<int>(rand2) << std::endl;

        std::cout << "Python init successs" << std::endl;
        Initialized = true;
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail: ")+perrorStr);
        Initialized=false;
        return;
    }
    LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("...Python init finished successful!"));
}

void TPyUBitmapClassifier::AUnInit(void)
{
 Initialized=false;
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPyUBitmapClassifier::ADefault(void)
{
 Initialized=false;
 NumClasses=4;
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyUBitmapClassifier::ABuild(void)
{
 return true;
}

// Сброс процесса счета без потери настроек
bool TPyUBitmapClassifier::AReset(void)
{
// Initialized = false;
 return true;
}

// Выполняет расчет этого объекта
bool TPyUBitmapClassifier::ACalculate(void)
{
// if(!Initialized)
// {
//    Init();
// }
 if(!InputImages.IsConnected())
  return true;

 if(InputImages->size()>0)
 {
     OutputClasses->clear();
     OutputClasses->resize(InputImages->size(), -1);
     OutputConfidences->Resize(InputImages->size(), NumClasses);
     for(int i=0; i<InputImages->size(); i++)
     {
         UBitmap &bmp = (*InputImages)[i];
         if(bmp.GetData()==NULL)
            continue;

         if(ImageColorModel!=bmp.GetColorModel())
         {
             LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("Incorrect image ["+sntoa(i)+"] color model. Need "+sntoa(*ImageColorModel)+" got: ")+sntoa(bmp.GetColorModel()));
             return true;
         }

         int w = bmp.GetWidth();
         int h = bmp.GetHeight();

         UBitmap b;
         b.SetRes(w, h, bmp.GetColorModel());
         bmp.CopyTo(0,0,b);

         cv::Mat m;
         if (b.GetColorModel() == RDK::ubmRGB24)
         {
             m=cv::Mat(b.GetHeight(), b.GetWidth(), CV_8UC3, b.GetData());
         }
         else if(b.GetColorModel() == RDK::ubmY8)
         {
             m=cv::Mat(b.GetHeight(), b.GetWidth(), CV_8U, b.GetData());
         }
         else
         {
             return true;
         }


         //RDK::SaveBitmapToFile("/home/ivan/testB.bmp", b);

         int object_cls = -1;
         /// Тут считаем
         try
         {
 //         import_array();
          py::object retval = IntegrationInterfaceInstance.attr("classify")(m);

          //std::vector<float> res = boost::python::extract<std::vector<float> >(retval);
          np::ndarray ndarr = boost::python::extract< np::ndarray  >(retval);
          int dms = ndarr.get_nd();

          if(dms>2)
          {
              LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyUBitmapClassifier error: Returned array with incorrect dimensions"));
              return true;
          }

          const Py_intptr_t* shp = ndarr.get_shape();
          long height = shp[0];
          long width  = shp[1];

          const Py_intptr_t *strides = ndarr.get_strides();
          long str0 = ndarr.strides(0);
          long str1 = ndarr.strides(1);

          std::vector<float> result;
          float *data = reinterpret_cast<float*>(ndarr.get_data());
          for(int y=0; y<height; y++)
          {
              for(int x=0; x<width;x++)
              {
                  float val = data[y*width+x];
                  result.push_back(val);
              }
          }
          //Если не совпадает то ничего не записываем и выдать ошибку!
          if(result.size()!=NumClasses)
          {
              LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyUBitmapClassifier error: NumClasses not equals to returned confidences count"));
              return true;
          }

          int max_id = -1;
          double max_conf = -100;
          for(int k=0; k<result.size(); k++)
          {
              (*OutputConfidences)(i, k) = result[k];
              if(result[k]>max_conf)
              {
                  max_conf = result[k];
                  max_id = k;
              }
          }
          (*OutputClasses)[i] = max_id;

         }
         catch (py::error_already_set const &)
         {
          std::string perrorStr = parse_python_exception();
          LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyUBitmapClassifier error: ")+perrorStr);
         } 
         catch(...)
         {
             LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception"));
         }
     }
 }

 //Пройти по всем агрегатам
 /*for(int i = 0; i < AggrRectsMatrix->GetRows(); ++i)
 {
  int object_cls = -1;
  /// Тут считаем
  try
  {
   py::object retval = IntegrationInterfaceInstance.attr("classify")(obj_rect);

   object_cls = boost::python::extract<int>(retval);
  }
  catch (py::error_already_set const &)
  {
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
 }*/
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



