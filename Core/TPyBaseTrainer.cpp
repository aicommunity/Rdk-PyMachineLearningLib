#ifndef RDK_TPyBaseTrainerCPP
#define RDK_TPyBaseTrainerCPP

#include "TPyBaseTrainer.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace RDK {

// ������
// --------------------------
// ������������ � �����������
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
  TrainingStatus("TrainingStatus",this),
  StartTraining("StartTraining",this),
  StopTraining("StopTraining",this),
  Epoch("Epoch",this),
  Progress("Progress",this),
  StopNow("StopNow",this)
{
}

TPyBaseTrainer::~TPyBaseTrainer(void)
{
}
// --------------------------


// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
bool TPyBaseTrainer::APythonInitialize(void)
{
    return true;
}

// �������������� �������� �� ��������� � ����� �������� �����
bool TPyBaseTrainer::APyDefault(void)
{
    return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TPyBaseTrainer::APyBuild(void)
{
    return true;
}

// ����� �������� ����� ��� ������ ��������
bool TPyBaseTrainer::APyReset(void)
{
    //TODO ��� ���-�� ����, �� ��� ���� �� ��������=( ��� ��������

    //��������� ��������
    Py_CUSTOM_BLOCK_THREADS
    IntegrationInterfaceInstance.attr("stop_now")();
    //Py_CUSTOM_UNBLOCK_THREADS

    StopTraining = StopNow = false;
    StartTraining = false;
    Py_CUSTOM_UNBLOCK_THREADS
    return true;
}

// ��������� ������ ����� �������
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



