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
//  PythonScriptPath("PythonScriptPath",this,&TPyComponent::SetPythonClassifierScriptPath),
  PythonModuleName("PythonModuleName",this, &TPyComponent::SetPythonModuleName),
  PythonClassName("PythonClassName",this, &TPyComponent::SetPythonClassName),
  UseFullPath("UseFullPath", this),
  PythonInitialized(false)
{
    Py_CUSTOM_BLOCK_THREADS
}

PyThreadState* TPyComponent::_custom_save = nullptr;
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
bool TPyComponent::SetPythonScriptFileName(const std::string& path)
{
 Ready=false;
 return true;
}
/*
bool TPyComponent::SetPythonClassifierScriptPath(const std::string& path)
{
 PythonScriptFileName = path;
 Ready=false;
 return true;
}
*/
bool TPyComponent::SetPythonModuleName(const std::string& path)
{
 Ready=false;
 return true;
}

bool TPyComponent::SetPythonClassName(const std::string& path)
{
 Ready=false;
 return true;
}


TPyComponent::~TPyComponent(void)
{
    Py_CUSTOM_BLOCK_THREADS
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
void TPyComponent::PythonInitialize(void)
{
    Py_CUSTOM_BLOCK_THREADS
    try
    {
        LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Python init started..."));
        PythonInitialized=false;

        if(PythonModuleName->empty())
        {
         LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("PythonModuleName is empty!"));
         return;
        }

        if(PythonClassName->empty())
        {
         LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("PythonClassName is empty!"));
         return;
        }

        if(!PyEval_ThreadsInitialized())
        {
         LogMessageEx(RDK_EX_FATAL,__FUNCTION__,std::string("Python Py_Initialize didn't called!"));
         LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail"));
         return;
        }

        py::object MainModule = py::import("__main__");  // импортируем main-scope, см. https://docs.python.org/3/library/__main__.html
        py::object MainNamespace = MainModule.attr("__dict__");  // извлекаем область имен

        // загрузка кода из файла в извлеченную область имен
        if(*UseFullPath)
        {
         FullPythonScriptFileName = PythonScriptFileName->c_str();
        }
        else
        {
         FullPythonScriptFileName = GetEnvironment()->GetCurrentDataDir()+PythonScriptFileName->c_str();
        }
        py::object DetectorInterfaceModule = import(*PythonModuleName,FullPythonScriptFileName,MainNamespace);

        // экземпляр питоновского класса, через который активируется виртуальная среда и загружается модель
        // TODO: пусть до среды брать из конфига
        IntegrationInterface = DetectorInterfaceModule.attr(PythonClassName->c_str());
        if(!IntegrationInterface.is_none())
         IntegrationInterfaceInstance = IntegrationInterface(); // DetectorEmbeddingInterface

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
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail: Unhandled exception"));
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
 UseFullPath=true;
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

// TODO добавить данную проверку на инициализацию в наследниках

// Выполняет расчет этого объекта
bool TPyComponent::ABeforeCalculate(void)
{
 if(!PythonInitialized)
    return true;

 return true;
}
// --------------------------

}
#endif



