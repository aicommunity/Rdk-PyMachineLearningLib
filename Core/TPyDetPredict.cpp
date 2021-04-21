#ifndef RDK_TPyDetPredictCPP
#define RDK_TPyDetPredictCPP

#include "TPyDetPredict.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyDetPredict::TPyDetPredict(void)
: WorkingDir("WorkingDir",this),
  ImagesDir("ImagesDir",this),
  ConfigPath("ConfigPath",this),
  WeightPath("WeightPath",this),
  StopNow("StopNow",this),
  StartPredict("StartPredict",this),
  PredictStatus("PredictStatus",this),
  ThreadIsAlive("ThreadIsAlive",this)
{
}

TPyDetPredict::~TPyDetPredict(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
TPyDetPredict* TPyDetPredict::New(void)
{
 return new TPyDetPredict;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
bool TPyDetPredict::APythonInitialize(void)
{
    return true;
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPyDetPredict::APyDefault(void)
{
    PythonModuleName="detection_train";
    PythonClassName="DetectionInterface";


    WorkingDir = "";
    ImagesDir = "";
    ConfigPath = "";
    WeightPath = "";
    StopNow = false;
    StartPredict = false;
    PredictStatus = 0;
    ThreadIsAlive = false;

    return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyDetPredict::APyBuild(void)
{
    return true;
}


// Сброс процесса счета без потери настроек
bool TPyDetPredict::APyReset(void)
{
    StopNow = false;
    StartPredict = false;

    //Остановка обучения
    //
    Py_BLOCK_GIL
    try
    {
        // Посылаем команду на остановку потока
        //IntegrationInterfaceInstance->attr("stop_now")();

        ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance->attr("get_thread_is_alive")());

        // Флаг оставноки потока именно в данном месте
        bool stopped = false;

        // Ждем пока поток завершиться
        while(ThreadIsAlive)
        {
            stopped = true;
            // Посылаем команду на остановку потока
            IntegrationInterfaceInstance->attr("stop_now")();

            //
            Py_UNBLOCK_GIL
            // даем возможность потоку завершиться
            Sleep(100);
            //
            Py_BLOCK_GIL

            ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance->attr("get_thread_is_alive")());
        }
        if(stopped)
        {
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyDetPredict reset: python thread stopped successful"));
        }
    }
    catch (py::error_already_set const &)
    {
        Py_UNBLOCK_GIL
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyDetPredict reset error: ")+perrorStr);
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception in TPyDetPredict reset:"));
    }
    Py_UNBLOCK_GIL
    return true;
}

// Выполняет расчет этого объекта
bool TPyDetPredict::ACalculate(void)
{
    // Если питон не проинициализирован, то ничего не делаем. Надо чтобы нажали Reset для повторной попытки иницилизации
    if(!PythonInitialized)
       return true;

    if(*WorkingDir != Environment->GetCurrentDataDir()+"Results/")
    {
        WorkingDir = Environment->GetCurrentDataDir()+"Results/";
    }

    gil_lock lock;
    try
    {   //Отключаем работу потоков питона (забираем GIL себе) для возмжности запуска функций

        // Проверка статуса выполнения
        py::object train_status = IntegrationInterfaceInstance->attr("is_predicting")();
        PredictStatus = boost::python::extract< int >(train_status);

        ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance->attr("get_thread_is_alive")());

        // Ошибка по время обучения (сообщаем и обнуляем статус)
        // Либо правильно сработало stop_now или stop_training
        if(PredictStatus == -1)
        {
            //сброс на случай выставления извне
            StartPredict = false;

            py::object except_string = IntegrationInterfaceInstance->attr("get_error_string")();

            std::string PyExceptionString = boost::python::extract< std::string >(except_string);

            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Exception or proper stop: ") + PyExceptionString);

            // Сброс статуса
            PredictStatus = 0;
            py::object res = IntegrationInterfaceInstance->attr("set_predicting_status_to_null")();
        }
        // Успешное завершение обучения. После обработки в компоненте сбрасывается в 0
        // Сообщаем и сбрасываем статус в 0
        if(PredictStatus == 2)
        {
            //сброс на случай выставления извне
            StartPredict = false;

            LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Prediction completed correctly"));

            // Сброс статуса
            PredictStatus = 0;
            py::object res = IntegrationInterfaceInstance->attr("set_predicting_status_to_null")();
        }
        // Если предсказание идет, опрашиваем геттеры
        if(PredictStatus == 1)
        {
            //сброс на случай выставления извне
            StartPredict = false;

            //Останавливаем предсказание, если требуется
            if(StopNow)
            {
                IntegrationInterfaceInstance->attr("stop_now")();
                LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Stop prediction initiated."));
                StopNow = false;
            }
        }
        // Если предсказание не идет и запуск возможен (статус 0)
        else
        {
            if(StartPredict)
            {
                if(ThreadIsAlive)
                {
                    LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python thread is alive. "
                                                                         "Set \"StopNow\" paramenter to true or activate \"Reset\". "
                                                                         "It will cause stopping of thread"));

                    StartPredict = false;
                    return true;
                }

                // Проверки на допустимость входных аргументов
                if(!CheckInputParameters())
                {
                    StartPredict = false;
                    return true;
                }

                LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Prediction started"));

                // Обнуление ненужных флагов
                StopNow = false;


                //Заполнение словаря параметров ( именованные аргументы)
                py::dict func_params;

                func_params["config"]       =   py::str(ConfigPath->c_str());
                func_params["weights"]      =   py::str(WeightPath->c_str());
                func_params["image_dir"]    =   py::str(ImagesDir->c_str());
                func_params["working_dir"]  =   py::str(WorkingDir->c_str());
                func_params["visualize"]    =   py::object(false);


                // Позиционные аргументы
                py::tuple data_dir_tuple = py::make_tuple();

                //Запуск обучения, внутри функции питона функция обучения отпускается в отдельный поток
                py::object retval = IntegrationInterfaceInstance->attr("predict_from_folder")
                                                                      (data_dir_tuple,
                                                                       func_params);

                // Проверка на исключительный случай
                // Если после выполнения функции predict_from_folder() сразу изменился TrainingStatus на -1
                py::object train_status = IntegrationInterfaceInstance->attr("train_status")();
                PredictStatus = boost::python::extract< int >(train_status);
                if(PredictStatus == -1)
                {
                    py::object except_string = IntegrationInterfaceInstance->attr("get_error_string")();

                    std::string PyExceptionString = boost::python::extract< std::string >(except_string);

                    LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Exception during start of predicting: ") + PyExceptionString);
                }

                PredictStatus = 1;
                StartPredict = false;
            }
        }
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyDetPredict error: ")+perrorStr);
        PredictStatus = 0;
        StartPredict = false;
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception"));
        PredictStatus = 0;
        StartPredict = false;
    }

    return true;
}


bool TPyDetPredict::CheckInputParameters()
{
    if(WorkingDir->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("WorkingDir parameter is empty!"));
        return false;
    }

    if(ConfigPath->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("ConfigPath parameter is empty!"));
        return false;
    }

    if(WeightPath->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("WeightPath parameter is empty!"));
        return false;
    }

    if(ImagesDir->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("ImagesDir parameter is empty!"));
        return false;
    }

    return true;
}

// --------------------------

}
#endif



