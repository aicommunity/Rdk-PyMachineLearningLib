#ifndef RDK_TPySegmenterTrainerCPP
#define RDK_TPySegmenterTrainerCPP

#include "TPySegmenterTrainer.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace RDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------
TPySegmenterTrainer::TPySegmenterTrainer(void)
: DatasetName("DatasetName",this),
  Model("Model",this),
  TrainingSize("TrainingSize",this),
  OutputSize("OutputSize",this),
  NumClasses("NumClasses",this),
  Classes("Classes",this),
  SplitBase("SplitBase",this),
  TrainList("TrainList",this),
  ValList("ValList",this),
  TestList("TestList",this),
  BatchSize("BatchSize",this),
  BatchesInEpoch("BatchesInEpoch",this),
  TestLoss("TestLoss",this),
  TestAcc("TestAcc",this),
  ValLoss("ValLoss",this),
  ValAcc("ValAcc",this),
  FPS("FPS",this)
{
}

TPySegmenterTrainer::~TPySegmenterTrainer(void)
{
}
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
TPySegmenterTrainer* TPySegmenterTrainer::New(void)
{
 return new TPySegmenterTrainer;
}
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
bool TPySegmenterTrainer::APythonInitialize(void)
{
    return true;
}

// �������������� �������� �� ��������� � ����� �������� �����
bool TPySegmenterTrainer::APyDefault(void)
{
    PythonModuleName="detection_train";
    PythonClassName="DetectionInterface";
    TrainDataDir = {""};
    WorkingDir = "";
    ArchitectureName= "SqueezeDet";
    SplitRatio = {70, 20, 10};
    SaveSplits = false;
    Epochs = 5;
    Weights = "";
    EarlyStop = 0;
    SavingInterval = 1;
    SaveBestOnly = false;
    TrainingStatus = false;
    StartTraining = false;
    StopTraining = false;

    DatasetName = "";
    Model = "";
    TrainingSize = 0;
    OutputSize = {0};
    NumClasses = 0;
    Classes = {""};
    SplitBase = false;
    TrainList = "";
    ValList = "";
    TestList = "";
    BatchSize = 0;
    BatchesInEpoch = 0;

    Epoch    = 0;
    Progress = 0.0;
    TestLoss = 0.0;
    TestAcc  = 0.0;
    ValLoss  = 0.0;
    ValAcc   = 0.0;
    FPS      = 0.0;

    return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TPySegmenterTrainer::APyBuild(void)
{
    return true;
}


// ��������� ������ ����� �������
bool TPySegmenterTrainer::ACalculate(void)
{
    // ���� ����� �� ������������������, �� ������ �� ������. ���� ����� ������ Reset ��� ��������� ������� ������������
    if(!PythonInitialized)
       return true;

    try
    {   //��������� ������ ������� ������ (�������� GIL ����) ��� ���������� ������� �������
        Py_CUSTOM_BLOCK_THREADS

        // �������� ������� ��������
        py::object train_status = IntegrationInterfaceInstance.attr("train_status")();
        TrainingStatus = boost::python::extract< int >(train_status);

        ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance.attr("get_thread_is_alive")());

        // ������ �� ����� �������� (�������� � �������� ������)
        if(TrainingStatus == -1)
        {
            //����� �� ������ ����������� �����
            StartTraining = false;

            py::object except_string = IntegrationInterfaceInstance.attr("get_error_string")();

            std::string PyExceptionString = boost::python::extract< std::string >(except_string);

            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Exception or proper stop: ") + PyExceptionString);

            // ����� �������
            TrainingStatus = 0;
            py::object res = IntegrationInterfaceInstance.attr("set_training_status_to_null")();

            // ����� ���������� ���������
            Epoch = 0;
            Progress = 0.0;

            TestLoss = 0.0;
            TestAcc  = 0.0;
            ValLoss  = 0.0;
            ValAcc   = 0.0;
            FPS      = 0.0;

        }
        // �������� ���������� ��������. ����� ��������� � ���������� ������������ � 0
        // �������� � ���������� ������ � 0
        if(TrainingStatus == 3)
        {
            //����� �� ������ ����������� �����
            StartTraining = false;

            LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Training completed or stopped correctly"));

            // ����� �������
            TrainingStatus = 0;
            py::object res = IntegrationInterfaceInstance.attr("set_training_status_to_null")();

            // ����� ���������� ���������
            Epoch = 0;
            Progress = 0.0;

            TestLoss = 0.0;
            TestAcc  = 0.0;
            ValLoss  = 0.0;
            ValAcc   = 0.0;
            FPS      = 0.0;
        }

        // ���� ��������/������������ ����, ���������� �������
        if(TrainingStatus == 1 || TrainingStatus == 2)
        {
            //����� �� ������ ����������� �����
            StartTraining = false;

            /*
             TODO ��������� ��������
             */

            //������������� �������� ���� ������ ��� �������, ���� ���������
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
        // ���� �������� �� ���� � ������ �������� (������ 0)
        else
        {
            if(StartTraining)
            {
                if(ThreadIsAlive)
                {
                    LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python thread is alive. "
                                                                         "Set \"StopNow\" paramenter to true or activate \"Reset\". "
                                                                         "It will cause stopping of thread"));
                    Py_CUSTOM_UNBLOCK_THREADS

                    StartTraining = false;
                    return true;
                }

                // �������� �� ������� ���������
                if(!CheckInputParameters())
                {
                    Py_CUSTOM_UNBLOCK_THREADS

                    StartTraining = false;
                    return true;
                }

                LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Training started"));

                // ��������� �������� ������
                StopTraining = StopNow = false;


                /*
                TODO ���������� ���������� � ����� �������� �������
                 */

                // �������� �� �������������� (����������� �����������) ������
                // ���� ����� ���������� ������� classification_train() ����� ��������� TrainingStatus �� -1
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
        StartTraining = false;
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception"));
        TrainingStatus = 0;
        StartTraining = false;
    }
    //��������� ������� �����������
    Py_CUSTOM_UNBLOCK_THREADS

    return true;
}


bool TPySegmenterTrainer::CheckInputParameters()
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

    // ���� ��� ����� � ����� - ������
    if(WorkingDir->back() != '/')
    {
        WorkingDir->push_back('/');
    }


    if(ArchitectureName->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("ArchitectureName is empty!"));
        return false;
    }


    if(SplitRatio.size()!=3)
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("SplitRatio must have 3 values!"));
        return false;
    }

    if(DatasetName->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("DatasetName is empty!"));
        return false;
    }

    // ��� ���������� �� ������ ���� ������
/*
    //�������� �� �������� ���������� WorkingDir
    if(!boost::filesystem::is_empty(WorkingDir->c_str()))
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("WorkingDir isn't empty, it contains some files"));
        return false;
    }
    */
    //TODO �������� ����� ��� �������� �� �����.�������� � ����.
    //TODO ��������� WorkingDir ��� �����-���� ��������
    //TODO �������� �� ���� ������������� � �.�.
    return true;
}

// --------------------------

}
#endif



