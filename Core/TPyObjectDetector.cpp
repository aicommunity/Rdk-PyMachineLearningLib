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


bool TPyObjectDetector::Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities)
{
 return true;
}

// --------------------------

}
#endif



