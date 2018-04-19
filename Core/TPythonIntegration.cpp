#ifndef RDK_TPythonIntegrationCPP
#define RDK_TPythonIntegrationCPP

#include "TPythonIntegration.h"

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


// --------------------------
// Скрытые методы управления счетом
// --------------------------
void TPythonIntegration::AInit(void)
{
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

 RDK::UBitmap& input_img=*InputImage;
 MDMatrix<int> &detections=*Detections;
 MDMatrix<int> &detection_class=*DetectionClass;
 MDMatrix<double> &detection_reliability=*DetectionReliability;

 /// Тут считаем

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
 }

 return true;
}
// --------------------------

}
#endif



