#ifndef RDK_TPyDetectorTrainerCPP
#define RDK_TPyDetectorTrainerCPP

#include "TPyDetectorTrainer.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyDetectorTrainer::TPyDetectorTrainer(void)
: DatasetType("DatasetType",this),
  Config("Config",this),
  SavePredicted("SavePredicted",this),
  Visualize("Visualize",this),
  PaintGt("PaintGt",this),
  LossNames("LossNames",this),
  TrainLosses("TrainLosses",this),
  ValLosses("ValLosses",this)
{
}

TPyDetectorTrainer::~TPyDetectorTrainer(void)
{
}
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
TPyDetectorTrainer* TPyDetectorTrainer::New(void)
{
 return new TPyDetectorTrainer;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
bool TPyDetectorTrainer::APythonInitialize(void)
{
    return true;
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPyDetectorTrainer::APyDefault(void)
{
    PythonModuleName="detection_train";
    PythonClassName="DetectionInterface";
    TrainDataDir = {""};
    WorkingDir = "";
    ArchitectureName= "SqueezeDet";
    SplitRatio = {0.7f, 0.2f, 0.1f};
    SaveSplits = false;
    Epochs = 5;
    Weights = "";
    EarlyStop = 0;
    SavingInterval = 1;
    SaveBestOnly = false;
    TrainingStatus = false;
    StartTraining = false;
    StopTraining = false;

    DatasetType = "";
    Config = "";
    SavePredicted = false;
    Visualize = 0;
    PaintGt = false;

    Epoch = 0;
    Progress = 0.0;

    LossNames = {""};
    ValLosses = {0.0};
    TrainLosses = {0.0};
    return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyDetectorTrainer::APyBuild(void)
{
    //_custom_save = nullptr;
    return true;
}


//TODO доразобраться с Py_BLOCK_THREADS и Py_UNBLOCK_THREADS
// Выполняет расчет этого объекта
bool TPyDetectorTrainer::ACalculate(void)
{
    if(!PythonInitialized)
       return true;

    try
    {   //Отключаем работу потоков питона (для возмонжости запуска функций) в конце включаем
        Py_CUSTOM_BLOCK_THREADS
        // Проверка статуса обучения
        py::object train_status = IntegrationInterfaceInstance.attr("train_status")();
        TrainingStatus = boost::python::extract< int >(train_status);

        // Ошибка по время обучения (сообщаем и обнуляем статус)
        if(TrainingStatus == -1)
        {
            //сброс на случай выставления извне
            StartTraining = false;

            py::object except_string = IntegrationInterfaceInstance.attr("get_error_string")();

            std::string PyExceptionString = boost::python::extract< std::string >(except_string);

            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Exception during function execution: ") + PyExceptionString);

            TrainingStatus = 0;
            py::object res = IntegrationInterfaceInstance.attr("set_training_status_to_null")();

            Epoch = 0;
            Progress = 0.0;

            LossNames = {""};
            ValLosses = {0.0};
            TrainLosses = {0.0};
        }
        // Успешное завершение работы. После обработки в компоненте сбрасывается в 0
        // (законечно обучение, либо успешное преждевременное завершение (stop_training, stop_now)
        // Сообщаем и сбрасываем статус в 0
        if(TrainingStatus == 3)
        {
            //сброс на случай выставления извне
            StartTraining = false;

            LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Training completed or stopped correctly"));

            TrainingStatus = 0;
            py::object res = IntegrationInterfaceInstance.attr("set_training_status_to_null")();

            Epoch = 0;
            Progress = 0.0;

            LossNames = {""};
            ValLosses = {0.0};
            TrainLosses = {0.0};
        }

        //если обучение/тестирование идет, опрашиваем геттеры
        if(TrainingStatus == 1 || TrainingStatus == 2)
        {
            //сброс на случай выставления извне
            StartTraining = false;

            //Запуск геттеров для получения информации о состоянии обучения
            py::object epoch        = IntegrationInterfaceInstance.attr("get_epoch")();
            py::object progress     = IntegrationInterfaceInstance.attr("get_progess")();

            py::object train_acc    = IntegrationInterfaceInstance.attr("get_train_acc")();
            py::object train_loss   = IntegrationInterfaceInstance.attr("get_train_loss")();
            py::object val_acc      = IntegrationInterfaceInstance.attr("get_val_acc")();
            py::object val_loss     = IntegrationInterfaceInstance.attr("get_val_loss")();

            Epoch       = boost::python::extract< int >(epoch);
            Progress    = boost::python::extract< float >(progress);

            TrainAcc    = boost::python::extract< float >(train_acc);
            TrainLoss   = boost::python::extract< float >(train_loss);
            ValAcc      = boost::python::extract< float >(val_acc);
            ValLoss     = boost::python::extract< float >(val_loss);


            //Останавливаем обучение либо вообще все, если требуется
            if(StopTraining)
            {
                IntegrationInterfaceInstance.attr("stop_training")();
                LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Stop training initiated. Trained networks will be tested"));
                StopTraining = false;
            }
            if(StopNow)
            {
                IntegrationInterfaceInstance.attr("stop_now")();
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
                // Проверки на входные аргументы
                if(!CheckInputParameters())
                {
                    Py_CUSTOM_UNBLOCK_THREADS
                    StartTraining = false;
                    return true;
                }

                LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Training started"));

                // Запуск обучения
                StopTraining = StopNow = false;

                // Перевод аргументов в тип PyObject для последующего вызова функции обучения
                py::list split_ratio;
                    split_ratio.append(SplitRatio[0]);
                    split_ratio.append(SplitRatio[1]);
                    split_ratio.append(SplitRatio[2]);

                py::list batch_sizes;
                    batch_sizes.append(BatchSizes[0]);
                    batch_sizes.append(BatchSizes[1]);
                    batch_sizes.append(BatchSizes[2]);

                py::object classes;
                if(!Classes.empty())
                {
                    split_ratio.append(Classes[0]);
                    split_ratio.append(Classes[1]);
                    split_ratio.append(Classes[2]);
                }

                //Заполнение словаря параметров (кроме data_dir у него свой кортеж)
                py::dict func_params;

                func_params["working_dir"]          =   py::str(WorkingDir->c_str());
                func_params["image_size"]           =   py::make_tuple(ImageSize[0],ImageSize[1],ImageSize[2]);
                func_params["architecture"]         =   py::str(ArchitectureName->c_str());
                func_params["dataset_name"]         =   py::str(DatasetName->c_str());
                func_params["split_ratio"]          =   split_ratio;
                func_params["save_splits"]          =   py::object(SaveSplits.v);
                func_params["copy_images"]          =   py::object(CopySplittedImages.v);
                func_params["test_equal_to_val"]    =   py::object(TestEqualVal.v);
                func_params["epochs"]               =   py::object(Epochs.v);
                func_params["learning_rate"]        =   py::object(LearningRate.v);
                func_params["batch_sizes"]          =   batch_sizes;
                func_params["weights"]              =   py::object(Weights->c_str());
                func_params["layers_to_be_trained"] =   LayersToBeTrained.v ? py::object(LayersToBeTrained.v) : py::str("default");
                func_params["classes"]              =   classes;
                func_params["early_stop"]           =   EarlyStop.v ? py::object(EarlyStop.v) : py::object(false);
                func_params["preprocessing"]        =   py::object();
                func_params["saving_interval"]      =   py::object(SavingInterval.v);
                func_params["save_best_only"]       =   py::object(SaveBestOnly.v);
                func_params["online_augmentation"]  =   py::dict();

                py::tuple data_dir_tuple = py::make_tuple((TrainDataDir->at(0).c_str()));

                //Запуск обучения, внутри функции питона остоединение обучения в поток
                py::object retval = IntegrationInterfaceInstance.attr("classification_train")
                                                                        (data_dir_tuple,
                                                                         func_params);

                // проверка на исключительный (практически невозможный) случай
                // если после выполнения функции classification_train() изменился TrainingStatus на -1
                py::object train_status = IntegrationInterfaceInstance.attr("train_status")();
                TrainingStatus = boost::python::extract< int >(train_status);
                if(TrainingStatus == -1)
                {
                    py::object except_string = IntegrationInterfaceInstance.attr("get_error_string")();

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
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception"));
        TrainingStatus = 0;
    }
    //Разрешаем потокам исполняться
    Py_CUSTOM_UNBLOCK_THREADS
    return true;
}


bool TPyDetectorTrainer::CheckInputParameters()
{
    if(TrainDataDir->empty() || TrainDataDir->at(0).empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("TrainDataDir is empty!"));
        return false;
    }

    if(WorkingDir->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("WorkingDir is empty!"));
        return false;
    }

    if(ArchitectureName->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("ArchitectureName is empty!"));
        return false;
    }

    if(DatasetType->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("DatasetType is empty!"));
        return false;
    }


    if(SplitRatio.size()!=3)
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("SplitRatio must have 3 values!"));
        return false;
    }

    //Проверка на непустую директорию WorkingDir
    if(!boost::filesystem::is_empty(WorkingDir.v.c_str()))
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("WorkingDir isn't empty, it contains some files"));
        return false;
    }
    //TODO возможно нужны еще проверки на отриц.значения и проч.
    //TODO создавать WorkingDir при каких-либо условиях
    //TODO проверки на пути относительные и т.д.
    return true;
}

// --------------------------

}
#endif



