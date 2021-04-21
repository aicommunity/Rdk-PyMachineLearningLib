#ifndef RDK_TPyClassifierTrainerCPP
#define RDK_TPyClassifierTrainerCPP

#include "TPyClassifierTrainer.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyClassifierTrainer::TPyClassifierTrainer(void)
: DatasetName("DatasetName",this),
  CopySplittedImages("CopySplittedImages",this),
  TestEqualVal("TestEqualVal",this),
  ImageSize("ImageSize",this),
  LearningRate("LearningRate",this),
  BatchSizes("BatchSizes",this),
  LayersToBeTrained("LayersToBeTrained",this),
  Classes("Classes",this),
  TrainLoss("TrainLoss",this),
  TrainAcc("TrainAcc",this),
  ValLoss("ValLoss",this),
  ValAcc("ValAcc",this)
{
}

TPyClassifierTrainer::~TPyClassifierTrainer(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
TPyClassifierTrainer* TPyClassifierTrainer::New(void)
{
 return new TPyClassifierTrainer;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
bool TPyClassifierTrainer::APythonInitialize(void)
{
    return true;
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPyClassifierTrainer::APyDefault(void)
{
    PythonModuleName="classifier_interface_tf1";
    PythonClassName="ClassificationInterface";

    // Общие параметры для всех тренеров
    TrainDataDir = {""};
    WorkingDir = "";
    ArchitectureName= "MobileNet";
    SplitRatio = {70,20,10};
    SaveSplits = false;
    Epochs = 5;
    Weights = "imagenet";
    EarlyStop = 0;
    SavingInterval = 1;
    SaveBestOnly = false;

    // Специфические параметры для обучения классификаторов
    DatasetName = "dataset_ft";
    CopySplittedImages = false;
    TestEqualVal = false;
    ImageSize = {224,224,3};
    Epochs = 5;
    LearningRate = 0.0002f;
    BatchSizes = {4,2,1};
    LayersToBeTrained = 0;
    Classes = {};


    TrainingStatus = 0;
    StartTraining = false;
    StopTraining = false;


    Epoch = 0;
    Progress = 0.0;

    TrainAcc = 0.0;
    TrainLoss = 0.0;
    ValAcc = 0.0;
    ValLoss = 0.0;

    return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyClassifierTrainer::APyBuild(void)
{
    return true;
}


// Выполняет расчет этого объекта
bool TPyClassifierTrainer::ACalculate(void)
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
        py::object train_status = IntegrationInterfaceInstance->attr("train_status")();
        TrainingStatus = boost::python::extract< int >(train_status);

        ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance->attr("get_thread_is_alive")());

        // Ошибка по время обучения (сообщаем и обнуляем статус)
        // Либо правильно сработало stop_now или stop_training
        if(TrainingStatus == -1)
        {
            //сброс на случай выставления извне
            StartTraining = false;

            py::object except_string = IntegrationInterfaceInstance->attr("get_error_string")();

            std::string PyExceptionString = boost::python::extract< std::string >(except_string);

            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Exception or proper stop: ") + PyExceptionString);

            // Сброс статуса
            TrainingStatus = 0;
            py::object res = IntegrationInterfaceInstance->attr("set_training_status_to_null")();

            // Сброс переменных состояний
            Epoch = 0;
            Progress = 0.0;

            TrainAcc = 0.0;
            TrainLoss = 0.0;
            ValAcc = 0.0;
            ValLoss = 0.0;
        }
        // Успешное завершение обучения. После обработки в компоненте сбрасывается в 0
        // Сообщаем и сбрасываем статус в 0
        if(TrainingStatus == 3)
        {
            //сброс на случай выставления извне
            StartTraining = false;

            LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Training completed correctly"));

            // Сброс статуса
            TrainingStatus = 0;
            py::object res = IntegrationInterfaceInstance->attr("set_training_status_to_null")();

            // Сброс переменных состояний
            Epoch = 0;
            Progress = 0.0;

            TrainAcc = 0.0;
            TrainLoss = 0.0;
            ValAcc = 0.0;
            ValLoss = 0.0;
        }
        // Если обучение/тестирование идет, опрашиваем геттеры
        if(TrainingStatus == 1 || TrainingStatus == 2)
        {
            //сброс на случай выставления извне
            StartTraining = false;

            //Запуск геттеров для получения информации о состоянии выполнения
            Epoch       = boost::python::extract< int >  (IntegrationInterfaceInstance->attr("get_epoch")());
            Progress    = boost::python::extract< float >(IntegrationInterfaceInstance->attr("get_progess")());

            TrainAcc    = boost::python::extract< float >(IntegrationInterfaceInstance->attr("get_train_acc")());
            TrainLoss   = boost::python::extract< float >(IntegrationInterfaceInstance->attr("get_train_loss")());
            ValAcc      = boost::python::extract< float >(IntegrationInterfaceInstance->attr("get_val_acc")());
            ValLoss     = boost::python::extract< float >(IntegrationInterfaceInstance->attr("get_val_loss")());


            //Останавливаем обучение либо вообще всю функцию, если требуется
            if(StopTraining)
            {
                IntegrationInterfaceInstance->attr("stop_training")();
                LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Stop training initiated. Trained networks will be tested"));
                StopTraining = false;
            }
            if(StopNow)
            {
                IntegrationInterfaceInstance->attr("stop_now")();
                LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Stop training/test initiated. Trained networks will not be tested. "
                                                                  "All python-execution will be stopped"));
                StopNow = false;
            }
        }
        // Если обучение не идет и запуск возможен (статус 0)
        else
        {
            if(StartTraining)
            {
                if(ThreadIsAlive)
                {
                    LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python thread is alive. "
                                                                         "Set \"StopNow\" paramenter to true or activate \"Reset\". "
                                                                         "It will cause stopping of thread"));

                    StartTraining = false;
                    return true;
                }

                // Проверки на допустимость входных аргументов
                if(!CheckInputParameters())
                {
                    StartTraining = false;
                    return true;
                }

                LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Training started"));

                // Обнуление ненужных флагов
                StopTraining = StopNow = false;

                // Подготовка аргументов для функции обучения

                // Перевод аргументов в нужные типы PyObject
                py::list split_ratio;
                    split_ratio.append(SplitRatio[0]);
                    split_ratio.append(SplitRatio[1]);
                    split_ratio.append(SplitRatio[2]);

                py::list batch_sizes;
                    batch_sizes.append(BatchSizes[0]);
                    batch_sizes.append(BatchSizes[1]);
                    batch_sizes.append(BatchSizes[2]);

                py::list classes;
                if(!Classes.empty())
                {
                    for(int i = 0; i < Classes.size(); i++)
                    {
                        classes.append(Classes[i]);
                    }
                }

                //Заполнение словаря параметров ( именованные аргументы)
                py::dict func_params;

                func_params["working_dir"]          =   py::str(WorkingDir->c_str());
                func_params["image_size"]           =   py::make_tuple(ImageSize[0],ImageSize[1],ImageSize[2]);
                func_params["architecture"]         =   py::str(ArchitectureName->c_str());
                func_params["dataset_name"]         =   py::str(DatasetName->c_str());
                func_params["split_ratio"]          =   split_ratio;
                func_params["save_splits"]          =   py::object(*SaveSplits);
                func_params["copy_images"]          =   py::object(*CopySplittedImages);
                func_params["test_equal_to_val"]    =   py::object(*TestEqualVal);
                func_params["epochs"]               =   py::object(*Epochs);
                func_params["learning_rate"]        =   py::object(*LearningRate);
                func_params["batch_sizes"]          =   batch_sizes;
                func_params["weights"]              =   py::object(Weights->c_str());
                func_params["layers_to_be_trained"] =   *LayersToBeTrained ? py::object(*LayersToBeTrained) : py::str("default");
                func_params["classes"]              =   classes;
                func_params["early_stop"]           =   *EarlyStop ? py::object(*EarlyStop) : py::object(false);
                func_params["preprocessing"]        =   py::object();
                func_params["saving_interval"]      =   py::object(*SavingInterval);
                func_params["save_best_only"]       =   py::object(*SaveBestOnly);
                func_params["online_augmentation"]  =   py::dict();

                // Позиционные аргументы
                py::tuple data_dir_tuple = py::make_tuple(TrainDataDir->at(0).c_str());

                //Запуск обучения, внутри функции питона функция обучения отпускается в отдельный поток
                py::object retval = IntegrationInterfaceInstance->attr("classification_train")
                                                                        (data_dir_tuple,
                                                                         func_params);

                // Проверка на исключительный (практически невозможный) случай
                // Если после выполнения функции classification_train() сразу изменился TrainingStatus на -1
                py::object train_status = IntegrationInterfaceInstance->attr("train_status")();
                TrainingStatus = boost::python::extract< int >(train_status);
                if(TrainingStatus == -1)
                {
                    py::object except_string = IntegrationInterfaceInstance->attr("get_error_string")();

                    std::string PyExceptionString = boost::python::extract< std::string >(except_string);

                    LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Exception during start of training: ") + PyExceptionString);
                }

                TrainingStatus = 1;
                StartTraining = false;
            }
        }
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyClassifierTrainer error: ")+perrorStr);
        TrainingStatus = 0;
        StartTraining = false;
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception"));
        TrainingStatus = 0;
        StartTraining = false;
    }

    return true;
}


bool TPyClassifierTrainer::CheckInputParameters()
{
    if(TrainDataDir->empty() || TrainDataDir->at(0).empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("TrainDataDir parameter is empty!"));
        return false;
    }

    if(ArchitectureName->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("ArchitectureName parameter is empty!"));
        return false;
    }

    if(DatasetName->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("DatasetName parameter is empty!"));
        return false;
    }

    if(SplitRatio.size()!=3)
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("SplitRatio must have 3 values!"));
        return false;
    }

    if(ImageSize.size()!=3)
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("ImageSize must have 3 values!"));
        return false;
    }

    if(BatchSizes.size()!=3)
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("BatchSizes must have 3 values!"));
        return false;
    }

    //Проверка на непустую директорию WorkingDir
    if(!boost::filesystem::is_empty(WorkingDir->c_str()))
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("WorkingDir isn't empty, it contains some files"));
        return false;
    }
    //TODO возможно нужны еще проверки на отриц.значения и проч.
    //TODO проверки на пути относительные и т.д.
    return true;
}

// --------------------------

}
#endif



