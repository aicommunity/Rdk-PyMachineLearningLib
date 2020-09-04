#define NO_IMPORT_ARRAY

#ifndef RDK_TPyObjectDetectorSqueezeDetCPP
#define RDK_TPyObjectDetectorSqueezeDetCPP

#include "TPyObjectDetectorSqueezeDet.h"
#include <iostream>

namespace RDK {


// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyObjectDetectorSqueezeDet::TPyObjectDetectorSqueezeDet(void):
  ConfigPath("ConfigPath",this),
  WeightsPath("WeightsPath",this)
{

}


TPyObjectDetectorSqueezeDet::~TPyObjectDetectorSqueezeDet(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
TPyObjectDetectorSqueezeDet* TPyObjectDetectorSqueezeDet::New(void)
{
 return new TPyObjectDetectorSqueezeDet;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
bool TPyObjectDetectorSqueezeDet::APythonInitialize(void)
{
    try
    {
        py::object initialize;
        initialize = IntegrationInterfaceInstance.attr("initialize_config")(GetEnvironment()->GetCurrentDataDir()+*ConfigPath,
                                                                            GetEnvironment()->GetCurrentDataDir()+*WeightsPath);


        if(!initialize.is_none())
        {
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init success"));
            return true;
        }
        else
        {
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python initialization failed"));
            return false;
        }
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail: ")+perrorStr);
        return false;
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail: Undandled exception"));
        return false;
    }

    LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("...Python init finished successful!"));
    return true;
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPyObjectDetectorSqueezeDet::APyDefault2(void)
{
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyObjectDetectorSqueezeDet::APyBuild2(void)
{
 return true;
}

// Сброс процесса счета без потери настроек
bool TPyObjectDetectorSqueezeDet::APyReset2(void)
{
 return true;
}

// Выполняет расчет этого объекта
// Выполняет обнаружение
bool TPyObjectDetectorSqueezeDet::Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities)
{
    if(!PythonInitialized)
        return false;
 // Тут считаем
 //std::vector<std::vector<double> > result;
 try
 {
  //import_array();
  py::object retval = IntegrationInterfaceInstance.attr("detect")(bmp);

  //std::vector<float> res = boost::python::extract<std::vector<float> >(retval);
  np::ndarray ndarr = boost::python::extract< np::ndarray  >(retval);
  int dms = ndarr.get_nd();

  if(dms>2)
  {
      //LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetectorSqueezeDet error: Returned array with incorrect dimensions"));
      return false;
  }

  if(dms==0)
  {
      //LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("TPyObjectDetectorSqueezeDet: Returned empty array"));
      return false;
  }

  const Py_intptr_t* shp = ndarr.get_shape();
  long height = shp[0];
  long width  = shp[1];

  if(width!=6)
  {
   //LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetectorSqueezeDet error: Script returned result in incorrect format"));
   return false;
  }
  const Py_intptr_t *strides = ndarr.get_strides();
  long str0 = ndarr.strides(0);
  long str1 = ndarr.strides(1);


//  result.resize(height);

  double *data = reinterpret_cast<double*>(ndarr.get_data());
  output_rects.Resize(height,4);
  output_classes.Resize(height,1);
  reliabilities.Resize(height,1);

  for(int y=0; y<height; y++)
  {
      for(int x=0; x<4;x++)
       output_rects(y,x)=data[y*width+x];
      reliabilities(y,0)=data[y*width+4];
      output_classes(y,0)=data[y*width+5];
  }
 }
 catch (py::error_already_set const &)
 {
  std::string perrorStr = parse_python_exception();
  LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetectorSqueezeDet error: ")+perrorStr);
 }

 return true;
}
// --------------------------

}
#endif



