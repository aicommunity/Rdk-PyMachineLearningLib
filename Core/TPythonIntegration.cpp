#ifndef RDK_TPythonIntegrationCPP
#define RDK_TPythonIntegrationCPP

#include "TPythonIntegration.h"

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPythonIntegration::TPythonIntegration(void)
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
// Восстановление настроек по умолчанию и сброс процесса счета
bool TPythonIntegration::ADefault(void)
{           
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
 return true;
}
// --------------------------

}
#endif



