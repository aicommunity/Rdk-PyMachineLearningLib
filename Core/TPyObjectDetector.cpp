#define NO_IMPORT_ARRAY

#ifndef RDK_TPyObjectDetectorCPP
#define RDK_TPyObjectDetectorCPP

#include "TPyObjectDetector.h"
#include <iostream>

namespace RDK {


// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyObjectDetector::TPyObjectDetector(void)
:ImageColorModel("ImageColorModel",this)
{

}


TPyObjectDetector::~TPyObjectDetector(void)
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
bool TPyObjectDetector::APyDefault(void)
{
 PythonModuleName="detector_interface";
 PythonClassName="DetectorEmbeddingInterface";

 return APyDefault2();
}

bool TPyObjectDetector::APyDefault2(void)
{
 return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyObjectDetector::APyBuild(void)
{
 return APyBuild2();
}

bool TPyObjectDetector::APyBuild2(void)
{
 return true;
}

// Сброс процесса счета без потери настроек
bool TPyObjectDetector::APyReset(void)
{
 return APyReset2();
}

bool TPyObjectDetector::APyReset2(void)
{
 return true;
}

// Выполняет расчет этого объекта
bool TPyObjectDetector::APyCalculate(void)
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

 try
 {
  if(!Detect(ProcessedBmp, *OutputRects, *OutputClasses, *OutputReliability))
  {
   ; // TODO: Тут что то должно быть
  }

  OutputObjects->Resize(OutputRects->GetRows(), 6);
  for(int i=0;i<OutputRects->GetRows();i++)
  {
   double wm = (*UseRelativeCoords)?(InputImage->GetWidth()):(1);
   double hm = (*UseRelativeCoords)?(InputImage->GetHeight()):(1);
   (*OutputObjects)(i,0) = (int)((*OutputRects)(i,0)*wm);
   (*OutputObjects)(i,1) = (int)((*OutputRects)(i,1)*hm);
   (*OutputObjects)(i,2) = (int)((*OutputRects)(i,2)*wm);
   (*OutputObjects)(i,3) = (int)((*OutputRects)(i,3)*hm);
   (*OutputObjects)(i,4)=(*OutputReliability)(i,0);
   (*OutputObjects)(i,5)=(*OutputClasses)(i,0);
  }

  if(UseDebugImage)
  {
   DebugImage->SetColorModel(ubmRGB24,false);
   InputImage->ConvertTo(*DebugImage);

   Graph.SetCanvas(DebugImage);

   UAFont *class_font=GetFont("Tahoma",14);

   for(int i=0; i<OutputRects->GetRows(); i++)
   {
      int xmin, ymin, xmax, ymax;

      xmin = (int)((*OutputObjects)(i,0));
      ymin = (int)((*OutputObjects)(i,1));
      xmax = (int)((*OutputObjects)(i,2));
      ymax = (int)((*OutputObjects)(i,3));



      double conf = (*OutputReliability)(i,0);
      int cls = (*OutputClasses)(i,0);


//      if(conf<0.2)
//        continue;

      Graph.SetPenColor(0x00FF00);
      Graph.Rect(xmin, ymin, xmax, ymax);



      std::stringstream ss;
      ss<<cls<<"["<<conf<<"]";

      if(class_font)
      {
        Graph.SetFont(class_font);
        Graph.Text(ss.str(),xmin, ymax+3);
      }
   }
  }
 }
 catch (py::error_already_set const &)
 {
  std::string perrorStr = parse_python_exception();
  LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetector error: ")+perrorStr);
 }

 return true;
}

bool TPyObjectDetector::Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities)
{
 return true;
}

// --------------------------

}
#endif



