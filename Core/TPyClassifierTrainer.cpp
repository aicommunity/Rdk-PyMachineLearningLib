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
: TrainDataDir("TrainDataDir",this),
  WorkingDir("WorkingDir",this),
  ArchitectureName("ArchitectureName",this),
  DatasetName("DatasetName",this),
  SplitRatio("SplitRatio",this),
  SaveSplits("SaveSplits",this),
  CopySplittedImages("CopySplittedImages",this),
  TestEqualVal("TestEqualVal",this),
  ImageSize("ImageSize",this),
  Epochs("Epochs",this),
  LearningRate("LearningRate",this),
  BatchSizes("BatchSizes",this),
  Weights("Weights",this),
  LayersToBeTrained("LayersToBeTrained",this),
  Classes("Classes",this),
  EarlyStop("EarlyStop",this),
  SavingInterval("SavingInterval",this),
  SaveBestOnly("SaveBestOnly",this),
  TrainingStatus("TrainingStatus",this),
  StartTraining("StartTraining",this),
  StopTraining("StopTraining",this),
  Epoch("Epoch",this),
  TrainAcc("TrainAcc",this),
  TrainLoss("TrainLoss",this),
  ValAcc("ValAcc",this),
  ValLoss("ValLoss",this),
  Progress("Progress",this),
  StopNow("StopNow",this)
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
    PythonClassName="ClassifierTrainer";
    TrainDataDir = "";
    WorkingDir = "";
    ArchitectureName= "MobileNet";
    DatasetName = "dataset_ft";
    SplitRatio = {70,20,10};
    SaveSplits = false;
    CopySplittedImages = false;
    TestEqualVal = false;
    ImageSize = {224,224,3};
    Epochs = 5;
    LearningRate = 0.0002f;
    BatchSizes = {4,2,1};
    Weights = "imagenet";
    LayersToBeTrained = 0;
    Classes = {};
    EarlyStop = 0;
    SavingInterval = 1;
    SaveBestOnly = false;
    TrainingStatus = false;
    StartTraining = false;
    StopTraining = false;
    Epoch = 0;
    TrainAcc = 0.0;
    TrainLoss = 0.0;
    ValAcc = 0.0;
    ValLoss = 0.0;
    Progress = 0.0;
    return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyClassifierTrainer::APyBuild(void)
{
    //_custom_save = nullptr;
    return true;
}

// Сброс процесса счета без потери настроек
bool TPyClassifierTrainer::APyReset(void)
{
    //TODO тут что-то надо, но как ниже не работает=(
    //Остановка обучения

    Py_CUSTOM_BLOCK_THREADS
    IntegrationInterfaceInstance.attr("stop_now")();
    //Py_CUSTOM_UNBLOCK_THREADS

    StopTraining = StopNow = false;
    StartTraining = false;

    return true;
}
//TODO доразобраться с Py_BLOCK_THREADS и Py_UNBLOCK_THREADS
// Выполняет расчет этого объекта
bool TPyClassifierTrainer::ACalculate(void)
{
    try
    {   //Отключаем работу потоков питона в конце включаем
        Py_CUSTOM_BLOCK_THREADS
        // Проверка статуса обучения
        py::object train_status = IntegrationInterfaceInstance.attr("train_status")();
        TrainingStatus.v = boost::python::extract< int >(train_status);

        //если обучение идет, опрашиваем геттеры
        if(TrainingStatus.v)
        {
            //Запуск геттеров для получения информации о состоянии обучения
            py::object epoch        = IntegrationInterfaceInstance.attr("get_epoch")();
            py::object train_acc    = IntegrationInterfaceInstance.attr("get_train_acc")();
            py::object train_loss   = IntegrationInterfaceInstance.attr("get_train_loss")();
            py::object val_acc      = IntegrationInterfaceInstance.attr("get_val_acc")();
            py::object val_loss     = IntegrationInterfaceInstance.attr("get_val_loss")();
            py::object progress     = IntegrationInterfaceInstance.attr("get_progess")();

            Epoch       = boost::python::extract< int >(epoch);
            TrainAcc    = boost::python::extract< float >(train_acc);
            TrainLoss   = boost::python::extract< float >(train_loss);
            ValAcc      = boost::python::extract< float >(val_acc);
            ValLoss     = boost::python::extract< float >(val_loss);
            Progress    = boost::python::extract< float >(progress);

            //Останавливаем обучение либо вообще все, если требуется
            if(StopTraining.v)
            {
                IntegrationInterfaceInstance.attr("stop_training")();
                StopTraining.v = false;
            }
            if(StopNow.v)
            {
                IntegrationInterfaceInstance.attr("stop_now")();
                StopNow.v = false;
            }
        }
        // Если обучение не идет - запускаем, если надо
        else
        {
            if(StartTraining.v)
            {
                // Проверки на входные аргументы
                if(!CheckInputParameters())
                {
                    Py_CUSTOM_UNBLOCK_THREADS
                    StartTraining = false;
                    return true;
                }

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

                func_params["working_dir"]          =   py::str(WorkingDir->c_str());;
                func_params["image_size"]           =   py::make_tuple(ImageSize[0],ImageSize[1],ImageSize[2]);
                func_params["architecture"]         =   py::str(ArchitectureName->c_str());;
                func_params["dataset_name"]         =   py::str(DatasetName->c_str());;
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

                py::tuple data_dir_tuple = py::make_tuple((TrainDataDir->c_str()));

                //Запуск обучения, внутри функции питона остоединение обучения в поток
                py::object retval = IntegrationInterfaceInstance.attr("classification_train")
                                                                        (data_dir_tuple,
                                                                         func_params);
                TrainingStatus = 1;
                StartTraining = false;

            }
            //return true;
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


bool TPyClassifierTrainer::CheckInputParameters()
{
    if(TrainDataDir->empty())
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

    if(DatasetName->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("DatasetName is empty!"));
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



