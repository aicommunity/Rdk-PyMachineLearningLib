#define NO_IMPORT_ARRAY

#ifndef RDK_TPyUBitmapClassifierCPP
#define RDK_TPyUBitmapClassifierCPP

#include "TPyUBitmapClassifier.h"
#include <iostream>

#define CLASS_UNDEFINED -2
#define CLASS_LOWQUAL -3

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------  //DetectionClass("DetectionClass",this),
TPyUBitmapClassifier::TPyUBitmapClassifier(void)
: InputImage("InputImage",this),
  InputImages("InputImages",this),
  OutputClasses("OutputClasses",this),
  ImageColorModel("ImageColorModel",this),
  NumClasses("NumClasses",this),
  ConfidenceThreshold("ConfidenceThreshold", this),
  OutputConfidences("OutputConfidences", this),
  ClassificationTime("ClassificationTime",this),
  WeightsPath("WeightsPath", this),
  UseWeightsPath("UseWeightsPath", this),
  UseRelativeWeightsPath("UseRelativeWeightsPath", this)
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
    if((*UseWeightsPath))
    {
        py::object initialize;
        if((*UseRelativeWeightsPath))
        {
            initialize = IntegrationInterfaceInstance.attr("initialize_weights")(GetEnvironment()->GetCurrentDataDir()+*WeightsPath);
            if(!initialize.is_none())
                return true;
            else
                return false;
        }
        else
        {
            initialize = IntegrationInterfaceInstance.attr("initialize_weights")(*WeightsPath);
            if(!initialize.is_none())
                return true;
            else
                return false;
        }

    }
    else
    {
        return true;
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
 ClassificationTime=0.0;
 OutputClasses->Resize(0,1);
 OutputConfidences->Resize(0, NumClasses);
 return true;
}

// Выполняет расчет этого объекта
bool TPyUBitmapClassifier::APyCalculate(void)
{
 ClassificationTime=0.0;
 clock_t start_frame = clock();

 if(InputImages.IsConnected() && InputImages->size()>0)
 {
     OutputClasses->Assign(InputImages->size(),1, CLASS_UNDEFINED);
     OutputConfidences->Assign(InputImages->size(), NumClasses,0.0);
     for(int i=0; i<InputImages->size(); i++)
     {
         UBitmap &bmp = (*InputImages)[i];
         if(bmp.GetData()==NULL)
         {
          LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("InputImages["+sntoa(i)+" is NULL"));
          continue;
         }


         if(ImageColorModel!=bmp.GetColorModel())
         {
          LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("Incorrect InputImages["+sntoa(i)+"] color model. Need "+sntoa(*ImageColorModel)+" got: ")+sntoa(bmp.GetColorModel()));
          continue;
         }

         MDVector<double> output_confidences;
         int class_id;
         bool is_classified;
         bool classify_res=ClassifyBitmap(bmp, output_confidences, ConfidenceThreshold, class_id, is_classified);

         if(classify_res)
         {
          for(int k=0; k<output_confidences.GetSize(); k++)
           (*OutputConfidences)(i, k) = output_confidences(k);

          (*OutputClasses)[i] = (is_classified)?class_id:CLASS_LOWQUAL;
         }
     }
 }
 else
 if(InputImage.IsConnected())
 {
  OutputClasses->Assign(1,1, CLASS_UNDEFINED);
  OutputConfidences->Assign(1, NumClasses,0.0);
  UBitmap &bmp = *InputImage;
  if(bmp.GetData() == NULL)
  {
   LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("InputImage is NULL"));
   return true;
  }

  if(ImageColorModel!=bmp.GetColorModel())
  {
   LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("Incorrect InputImage color model. Need "+sntoa(*ImageColorModel)+" got: ")+sntoa(bmp.GetColorModel()));
   return true;
  }

  MDVector<double> output_confidences;
  int class_id;
  bool is_classified;
  bool classify_res=ClassifyBitmap(bmp, output_confidences, ConfidenceThreshold, class_id, is_classified);

  for(int k=0; k<output_confidences.GetSize(); k++)
   (*OutputConfidences)(0, k) = output_confidences(k);

  (*OutputClasses)[0] = (is_classified)?class_id:CLASS_LOWQUAL;
 }
 clock_t end_frame = clock();
 ClassificationTime = ((double) (end_frame - start_frame)) / CLOCKS_PER_SEC;
 return true;
}


/// Обрабатывает одно изображение
bool TPyUBitmapClassifier::ClassifyBitmap(UBitmap &bmp, MDVector<double> &output_confidences, double conf_thresh, int &class_id, bool &is_classified)
{
 int w = bmp.GetWidth();
 int h = bmp.GetHeight();

 ProcessedBmp.SetRes(w, h, bmp.GetColorModel());
 bmp.CopyTo(0,0,ProcessedBmp);

 if (ProcessedBmp.GetColorModel() == RDK::ubmRGB24)
 {
     ProcessedMat=cv::Mat(ProcessedBmp.GetHeight(), ProcessedBmp.GetWidth(), CV_8UC3, ProcessedBmp.GetData());
     cv::cvtColor(ProcessedMat, ProcessedMat, CV_BGR2RGB);
 }
 else
 if(ProcessedBmp.GetColorModel() == RDK::ubmY8)
 {
     ProcessedMat=cv::Mat(ProcessedBmp.GetHeight(), ProcessedBmp.GetWidth(), CV_8U, ProcessedBmp.GetData());
 }
 else
 {
  LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyUBitmapClassifier error: Incorrect UBitmap color model: ")+sntoa(ProcessedBmp.GetColorModel()));
  return false;
 }

 int object_cls = -1;
 /// Тут считаем
 try
 {
  clock_t start = clock();
  py::object retval = IntegrationInterfaceInstance.attr("classify")(ProcessedMat);
  clock_t end = clock();
  double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  //std::vector<float> res = boost::python::extract<std::vector<float> >(retval);
  printf("classification took %f seconds to execute \n", cpu_time_used);
  np::ndarray ndarr = boost::python::extract< np::ndarray  >(retval);
  int dms = ndarr.get_nd();

  if(dms>2)
  {
   LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyUBitmapClassifier error: Returned array with incorrect dimensions"));
   return false;
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
  if(result.size() != NumClasses)
  {
   LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyUBitmapClassifier error: NumClasses "+sntoa(*NumClasses)+" not equals to returned confidences count "+sntoa(result.size())));
   return false;
  }

  int max_id = -1;
  double max_conf = -100;

  output_confidences.Resize(result.size());
  for(int k=0; k<result.size(); k++)
  {
      output_confidences(k) = result[k];
      if(result[k]>max_conf)
      {
          max_conf = result[k];
          max_id = k;
      }
  }

  if(max_conf<conf_thresh)
  {
//      for(int k=0; k<result.size(); k++)
//          result[k]=0.0f;

//      max_id=CLASS_LOWQUAL;
      is_classified=false;
  }
  else
   is_classified=true;

  class_id = max_id;

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
 return true;
}
// --------------------------

}
#endif



