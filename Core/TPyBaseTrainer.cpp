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
    // ������������� ���������� ���������� ������ ������
    Py_BLOCK_GIL
    try
    {
        // �������� ������� �� ��������� ����������
        IntegrationInterfaceInstance.attr("stop_now")();

        ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance.attr("get_thread_is_alive")());

        // ���� ��������� ������ ������ � ������ �����
        bool stopped = false;

        // ���� ���� ����� �����������
        while(ThreadIsAlive)
        {
            // �������� ������� �� ��������� ������
            IntegrationInterfaceInstance.attr("stop_now")();

            stopped = true;

            Py_UNBLOCK_GIL
            // ���� ����������� ������ �����������
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
    StopTraining = StopNow = false;
    StartTraining = false;

    //��������� ��������
    //
    Py_BLOCK_GIL
    try
    {
        // �������� ������� �� ��������� ������
        IntegrationInterfaceInstance.attr("stop_now")();

        ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance.attr("get_thread_is_alive")());

        // ���� ��������� ������ ������ � ������ �����
        bool stopped = false;

        // ���� ���� ����� �����������
        while(ThreadIsAlive)
        {
            stopped = true;
            // �������� ������� �� ��������� ������
            IntegrationInterfaceInstance.attr("stop_now")();

            //
            Py_UNBLOCK_GIL
            // ���� ����������� ������ �����������
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



