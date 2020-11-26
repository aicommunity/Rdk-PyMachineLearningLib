#ifndef RDK_TPyBaseTrainerCPP
#define RDK_TPyBaseTrainerCPP

#include "TPyBaseTrainer.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyBaseTrainer::TPyBaseTrainer(void)
: TrainDataDir("TrainDataDir",this),
  WorkingDir("WorkingDir",this),
  ArchitectureName("ArchitectureName",this),
  SplitRatio("SplitRatio",this),
  SaveSplits("SaveSplits",this),
  Epochs("Epochs",this),
  Weights("Weights",this),
  EarlyStop("EarlyStop",this),
  SavingInterval("SavingInterval",this),
  SaveBestOnly("SaveBestOnly",this),
  StartTraining("StartTraining",this),
  StopTraining("StopTraining",this),
  StopNow("StopNow",this),
  TrainingStatus("TrainingStatus",this),
  ThreadIsAlive("ThreadIsAlive",this),
  Epoch("Epoch",this),
  Progress("Progress",this)
{
}

TPyBaseTrainer::~TPyBaseTrainer(void)
{
    // Гарантировано дожидаемся завершения потока питона
    Py_BLOCK_GIL
    try
    {
        // Посылаем команду на остановку исполнения
        IntegrationInterfaceInstance.attr("stop_now")();

        ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance.attr("get_thread_is_alive")());

        // Флаг оставноки потока именно в данном месте
        bool stopped = false;

        // Ждем пока поток завершиться
        while(ThreadIsAlive)
        {
            // Посылаем команду на остановку потока
            IntegrationInterfaceInstance.attr("stop_now")();

            stopped = true;

            Py_UNBLOCK_GIL
            // даем возможность потоку завершиться
            Sleep(100);
            Py_BLOCK_GIL

            ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance.attr("get_thread_is_alive")());
        }
        if(stopped)
        {
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyBaseTrainer desctruction: python thread stopped successful"));
        }
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyBaseTrainer destructor error: ")+perrorStr);
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception in TPyBaseTrainer destructor:"));
    }
    Py_UNBLOCK_GIL
}
// --------------------------


// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
bool TPyBaseTrainer::APythonInitialize(void)
{
    return true;
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPyBaseTrainer::APyDefault(void)
{
    return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyBaseTrainer::APyBuild(void)
{
    return true;
}

// Сброс процесса счета без потери настроек
bool TPyBaseTrainer::APyReset(void)
{
    StopTraining = StopNow = false;
    StartTraining = false;

    //Остановка обучения
    //
    Py_BLOCK_GIL
    try
    {
        // Посылаем команду на остановку потока
        IntegrationInterfaceInstance.attr("stop_now")();

        ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance.attr("get_thread_is_alive")());

        // Флаг оставноки потока именно в данном месте
        bool stopped = false;

        // Ждем пока поток завершиться
        while(ThreadIsAlive)
        {
            stopped = true;
            // Посылаем команду на остановку потока
            IntegrationInterfaceInstance.attr("stop_now")();

            //
            Py_UNBLOCK_GIL
            // даем возможность потоку завершиться
            Sleep(100);
            //
            Py_BLOCK_GIL

            ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance.attr("get_thread_is_alive")());
        }
        if(stopped)
        {
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyBaseTrainer reset: python thread stopped successful"));
        }
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyBaseTrainer reset error: ")+perrorStr);
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception in TPyBaseTrainer reset:"));
    }
    Py_UNBLOCK_GIL
    return true;
}

// Выполняет расчет этого объекта
bool TPyBaseTrainer::ACalculate(void)
{
    return true;
}

bool TPyBaseTrainer::CheckInputParameters()
{
    return true;
}

// --------------------------

}
#endif



