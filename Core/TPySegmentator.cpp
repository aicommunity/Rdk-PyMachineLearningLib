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
TPySegmentator::TPySegmentator(void):
  ImageColorModel("ImageColorModel",this)
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


bool TPySegmentator::Inference(UBitmap &bmp, UBitmap &mask)
{
 return true;
}

// --------------------------

}
#endif



