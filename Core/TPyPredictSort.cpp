#ifndef RDK_TPyPredictSortCPP
#define RDK_TPyPredictSortCPP

#include "TPyPredictSort.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyPredictSort::TPyPredictSort(void)
: WorkingDir("WorkingDir",this),
  ImagesDir("ImagesDir",this),
  SortImages("SortImages",this),
  ConfigPath("ConfigPath",this),
  WeightPath("WeightPath",this),
  StopNow("StopNow",this),
  StartPredict("StartPredict",this),
  PredictStatus("PredictStatus",this),
  ThreadIsAlive("ThreadIsAlive",this)
{
}

TPyPredictSort::~TPyPredictSort(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
TPyPredictSort* TPyPredictSort::New(void)
{
 return new TPyPredictSort;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
bool TPyPredictSort::APythonInitialize(void)
{
    return true;
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPyPredictSort::APyDefault(void)
{
    PythonModuleName="classifier_interface_tf1";
    PythonClassName="ClassificationInterface";


    WorkingDir = "";
    ImagesDir = "";
    ConfigPath = "";
    WeightPath = "";
    SortImages = false;
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
bool TPyPredictSort::APyBuild(void)
{
    return true;
}


// Сброс процесса счета без потери настроек
bool TPyPredictSort::APyReset(void)
{
    StopNow = false;
    StartPredict = false;

    //Остановка обучения
    //
    Py_BLOCK_GIL
    try
    {
        // Посылаем команду на остановку потока
        IntegrationInterfaceInstance->attr("stop_now")();

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
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyPredictSort reset: python thread stopped successful"));
        }
    }
    catch (py::error_already_set const &)
    {
        Py_UNBLOCK_GIL
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyPredictSort reset error: ")+perrorStr);
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception in TPyPredictSort reset:"));
    }
    Py_UNBLOCK_GIL
    return true;
}

// Выполняет расчет этого объекта
bool TPyPredictSort::ACalculate(void)
{
    // Если питон не проинициализирован, то ничего не делаем. Надо чтобы нажали Reset для повторной попытки иницилизации
    if(!PythonInitialized)
       return true;
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

            LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Training completed correctly"));

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

                LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Training started"));

                // Обнуление ненужных флагов
                StopNow = false;


                //Запуск обучения, внутри функции питона функция обучения отпускается в отдельный поток
                py::object retval = IntegrationInterfaceInstance->attr("predict_and_sort")
                                                                        (py::str(ConfigPath->c_str()),
                                                                         py::str(WeightPath->c_str()),
                                                                         py::str(ImagesDir->c_str()),
                                                                         py::str(WorkingDir->c_str()),
                                                                         py::object(true),
                                                                         py::object(*SortImages));

                // Проверка на исключительный (практически невозможный) случай
                // Если после выполнения функции classification_train() сразу изменился TrainingStatus на -1
                py::object train_status = IntegrationInterfaceInstance->attr("train_status")();
                PredictStatus = boost::python::extract< int >(train_status);
                if(PredictStatus == -1)
                {
                    py::object except_string = IntegrationInterfaceInstance->attr("get_error_string")();

                    std::string PyExceptionString = boost::python::extract< std::string >(except_string);

                    LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Exception during start of predictiong: ") + PyExceptionString);
                }

                PredictStatus = 1;
                StartPredict = false;
            }
        }
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyClassifierTrainer error: ")+perrorStr);
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


bool TPyPredictSort::CheckInputParameters()
{
    if(WorkingDir->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("WorkingDir is empty!"));
        return false;
    }

    if(ConfigPath->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("WorkingDir is empty!"));
        return false;
    }

    if(WeightPath->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("WorkingDir is empty!"));
        return false;
    }

    if(ImagesDir->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("WorkingDir is empty!"));
        return false;
    }

    return true;
}

// --------------------------

}
#endif



