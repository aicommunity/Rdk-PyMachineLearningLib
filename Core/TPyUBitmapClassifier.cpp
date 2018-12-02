#define NO_IMPORT_ARRAY

#ifndef RDK_TPyUBitmapClassifierCPP
#define RDK_TPyUBitmapClassifierCPP

#include "TPyUBitmapClassifier.h"
#include <iostream>

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------  //DetectionClass("DetectionClass",this),
TPyUBitmapClassifier::TPyUBitmapClassifier(void)
: InputImages("InputImages",this),
  OutputClasses("OutputClasses",this),
  ImageColorModel("ImageColorModel",this),
  NumClasses("NumClasses",this),
  OutputConfidences("OutputConfidences", this)
{
}

TPyUBitmapClassifier::~TPyUBitmapClassifier(void)
{
}
// --------------------------

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
bool TPyUBitmapClassifier::APythonInitialize(void)
{
    UBitmap b;
    b.SetRes(10, 10, static_cast<RDK::UBMColorModel>(*ImageColorModel));
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

    try
    {
     py::object retval = IntegrationInterfaceInstance.attr("classify")(m);
    }
    catch (py::error_already_set const &)
    {
     std::string perrorStr = parse_python_exception();
     LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyUBitmapClassifier error: ")+perrorStr);
     return false;
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception"));
        return false;
    }
 return true;
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPyUBitmapClassifier::APyDefault(void)
{
 PythonModuleName="classifier_interface";
 PythonClassName="ClassifierEmbeddingInterface";
 NumClasses=4;

 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyUBitmapClassifier::APyBuild(void)
{
 return true;
}

// Сброс процесса счета без потери настроек
bool TPyUBitmapClassifier::APyReset(void)
{
 return true;
}

// Выполняет расчет этого объекта
bool TPyUBitmapClassifier::APyCalculate(void)
{
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

 return true;
}
// --------------------------

}
#endif



