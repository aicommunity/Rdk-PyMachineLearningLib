#define NO_IMPORT_ARRAY

#ifndef RDK_TPyComponentCPP
#define RDK_TPyComponentCPP

#include "TPyComponent.h"
#include <iostream>

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyComponent::TPyComponent(void)
: PythonScriptFileName("PythonScriptFileName",this,&TPyComponent::SetPythonScriptFileName),
  PythonScriptPath("PythonScriptPath",this,&TPyComponent::SetPythonClassifierScriptPath),
  PythonInitialized(false)
{
}
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
bool TPyComponent::SetPythonScriptFileName(const std::string& path)
{
 Ready=false;
 return true;
}

bool TPyComponent::SetPythonClassifierScriptPath(const std::string& path)
{
 PythonScriptFileName = path;
 Ready=false;
 return true;
}

TPyComponent::~TPyComponent(void)
{
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
void TPyComponent::PythonInitialize(void)
{
    try
    {
        LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Python init started..."));
        PythonInitialized=false;
        py::object MainModule = py::import("__main__");  // импортируем main-scope, см. https://docs.python.org/3/library/__main__.html
        py::object MainNamespace = MainModule.attr("__dict__");  // извлекаем область имен

        // загрузка кода из файла в извлеченную область имен
        FullPythonScriptFileName = GetEnvironment()->GetCurrentDataDir()+PythonScriptFileName->c_str();
        py::object DetectorInterfaceModule = import("detector_interface",FullPythonScriptFileName,MainNamespace);

        // экземпляр питоновского класса, через который активируется виртуальная среда и загружается модель
        // TODO: пусть до среды брать из конфига
        IntegrationInterface = DetectorInterfaceModule.attr("DetectorEmbeddingInterface");
        if(!IntegrationInterface.is_none())
         IntegrationInterfaceInstance = IntegrationInterface();

        PythonInitialized=APythonInitialize();
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail: ")+perrorStr);
        PythonInitialized=false;
    }
    catch(...)
    {
        PythonInitialized=false;
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail: Undandled exception"));
    }

    if(PythonInitialized)
     LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("...Python init finished successful!"));
    else
     LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("...Python init FAILED!"));
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPyComponent::ADefault(void)
{
 PythonInitialized=false;
 return APyDefault();
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyComponent::ABuild(void)
{
 if(IsInit())
  PythonInitialize();
 return APyBuild();
}

// Сброс процесса счета без потери настроек
bool TPyComponent::AReset(void)
{
 if(!PythonInitialized)
  PythonInitialize();
 return APyReset();
}

// Выполняет расчет этого объекта
bool TPyComponent::ACalculate(void)
{
 if(!PythonInitialized)
  return true;
 return APyCalculate();
}
// --------------------------

}
#endif



