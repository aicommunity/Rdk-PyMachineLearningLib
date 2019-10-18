#define NO_IMPORT_ARRAY

#ifndef RDK_TPySegmentatorCPP
#define RDK_TPySegmentatorCPP

#include "TPySegmentator.h"
#include <iostream>

namespace RDK {


// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPySegmentator::TPySegmentator(void)
: InputImage("InputImage",this),
  ImageColorModel("ImageColorModel",this),
  ClassColors("ClassColors", this),
  OutputImage("DebugImage",this)


{

}


TPySegmentator::~TPySegmentator(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса

// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool TPySegmentator::APyDefault(void)
{
 PythonModuleName="segmentator_interface";
 PythonClassName="SegmentatorEmbeddingInterface";

 return APyDefault2();
}

bool TPySegmentator::APyDefault2(void)
{
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPySegmentator::APyBuild(void)
{
 return APyBuild2();
}

bool TPySegmentator::APyBuild2(void)
{
 return true;
}

// Сброс процесса счета без потери настроек
bool TPySegmentator::APyReset(void)
{
 return APyReset2();
}

bool TPySegmentator::APyReset2(void)
{
 return true;
}

// Выполняет расчет этого объекта
bool TPySegmentator::APyCalculate(void)
{
 if(!InputImage.IsConnected())
  return true;

 if(ImageColorModel!=InputImage->GetColorModel())
 {
     LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("Incorrect input image color model. Need "+sntoa(*ImageColorModel)+" got: ")+sntoa(InputImage->GetColorModel()));
     return true;
 }

 ProcessedBmp=*InputImage;

 if(ProcessedBmp.GetData()==NULL)
  return true;

 UBitmap &mask = (*OutputImage);

 try
 {
  if(!Inference(ProcessedBmp, mask))
  {
   ; // TODO: Тут что то должно быть
  }
 }
 catch (py::error_already_set const &)
 {
  std::string perrorStr = parse_python_exception();
  LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPySegmentator error: ")+perrorStr);
 }

 return true;
}

bool TPySegmentator::Inference(UBitmap &bmp, UBitmap &mask)
{
 return true;
}

// --------------------------

}
#endif



