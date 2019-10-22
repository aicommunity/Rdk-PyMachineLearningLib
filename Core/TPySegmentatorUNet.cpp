#define NO_IMPORT_ARRAY

#ifndef RDK_TPySegmentatorUNetCPP
#define RDK_TPySegmentatorUNetCPP

#include "TPySegmentatorUNet.h"
#include <iostream>

namespace RDK {


// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPySegmentatorUNet::TPySegmentatorUNet(void):
  WeightsPath("WeightsPath",this)
{

}


TPySegmentatorUNet::~TPySegmentatorUNet(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
TPySegmentatorUNet* TPySegmentatorUNet::New(void)
{
 return new TPySegmentatorUNet;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
bool TPySegmentatorUNet::APythonInitialize(void)
{
    try
    {
        py::object initialize;
        initialize = IntegrationInterfaceInstance.attr("initialize_config")(GetEnvironment()->GetCurrentDataDir()+*WeightsPath);


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
bool TPySegmentatorUNet::APyDefault2(void)
{
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPySegmentatorUNet::APyBuild2(void)
{
 return true;
}

// Сброс процесса счета без потери настроек
bool TPySegmentatorUNet::APyReset2(void)
{
 return true;
}

// Выполняет расчет этого объекта
// Выполняет обнаружение
bool TPySegmentatorUNet::Inference(UBitmap &bmp, UBitmap &mask)
{
 try
 {
  py::object retval = IntegrationInterfaceInstance.attr("inference")(bmp);

  cv::Mat result_mat = pbcvt::fromNDArrayToMat(retval.ptr());

  int type = result_mat.type();
  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);
  if(depth==CV_8U && chans==3)
  {
      UBitmap temp;
      temp.SetRes(result_mat.cols, result_mat.rows, RDK::ubmRGB24);
      temp.AttachBuffer(result_mat.data);
      mask.SetRes(result_mat.cols, result_mat.rows, RDK::ubmRGB24);
      temp.ConvertTo(mask);
      temp.DetachBuffer();
      mask.SwapRGBChannels();
  }
  else if(depth==CV_8U && chans==1)
  {
      UBitmap temp;
      temp.SetRes(result_mat.cols, result_mat.rows, RDK::ubmY8);
      temp.AttachBuffer(result_mat.data);
      mask.SetRes(result_mat.cols, result_mat.rows, RDK::ubmY8);
      temp.ConvertTo(mask);
  }
  else
  {
    LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPySegmentatorUNet: Received numpy array with incorrect color model"));
  }

 }
 catch (py::error_already_set const &)
 {
  std::string perrorStr = parse_python_exception();
  LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPySegmentatorUNet error: ")+perrorStr);
 }

 return true;
}
// --------------------------

}
#endif



