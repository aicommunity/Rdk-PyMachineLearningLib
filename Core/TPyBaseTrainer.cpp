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
  Epoch("Epoch",this),
  Progress("Progress",this)
{
}

TPyBaseTrainer::~TPyBaseTrainer(void)
{
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
    //TODO тут что-то надо, но как ниже не работает=( уже работает

    //Остановка обучения
    Py_CUSTOM_BLOCK_THREADS
    IntegrationInterfaceInstance.attr("stop_now")();

    StopTraining = StopNow = false;
    StartTraining = false;
    Py_CUSTOM_UNBLOCK_THREADS
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



