#ifndef RDK_TPyDetectorTrainerCPP
#define RDK_TPyDetectorTrainerCPP

#include "TPyDetectorTrainer.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace RDK {

// ������
// --------------------------
// ������������ � �����������
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
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
TPyDetectorTrainer* TPyDetectorTrainer::New(void)
{
 return new TPyDetectorTrainer;
}
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
bool TPyDetectorTrainer::APythonInitialize(void)
{
    return true;
}

// �������������� �������� �� ��������� � ����� �������� �����
bool TPyDetectorTrainer::APyDefault(void)
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

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TPyDetectorTrainer::APyBuild(void)
{
    return true;
}


//TODO ������������� � Py_BLOCK_THREADS � Py_UNBLOCK_THREADS
// ��������� ������ ����� �������
bool TPyDetectorTrainer::ACalculate(void)
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

            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Exception during python function execution: ") + PyExceptionString);

            // ����� �������
            TrainingStatus = 0;
            py::object res = IntegrationInterfaceInstance.attr("set_training_status_to_null")();

            // ����� ���������� ���������
            Epoch = 0;
            Progress = 0.0;

            LossNames = {""};
            ValLosses = {0.0};
            TrainLosses = {0.0};

        }
        // �������� ���������� ������. ����� ��������� � ���������� ������������ � 0
        // (��������� ��������, ���� �������� ��������������� ���������� (stop_training, stop_now)
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

            LossNames = {""};
            ValLosses = {0.0};
            TrainLosses = {0.0};
        }

        // ���� ��������/������������ ����, ���������� �������
        if(TrainingStatus == 1 || TrainingStatus == 2)
        {
            //����� �� ������ ����������� �����
            StartTraining = false;

            //������ �������� ��� ��������� ���������� � ��������� ����������
            Epoch       = boost::python::extract< int >  (IntegrationInterfaceInstance.attr("get_epoch")());
            Progress    = boost::python::extract< float >(IntegrationInterfaceInstance.attr("get_progess")());

            // ������ ��������� ������
            py::list loss_names   = boost::python::extract< py::list >(IntegrationInterfaceInstance.attr("get_loss_names")());
            py::list val_losses   = boost::python::extract< py::list >(IntegrationInterfaceInstance.attr("get_val_losses")());
            py::list train_losses = boost::python::extract< py::list >(IntegrationInterfaceInstance.attr("get_train_losses")());

            // ��������� ������, ���� ���������� ������ �� ������
            if(loss_names != py::list())
            {
                LossNames->clear();
                for (int i = 0; i < py::len(loss_names); ++i)
                {
                    LossNames->push_back(boost::python::extract<std::string>(loss_names[i]));
                }
            }
            if(val_losses != py::list())
            {
                ValLosses->clear();
                for (int i = 0; i < py::len(val_losses); ++i)
                {
                    ValLosses->push_back(boost::python::extract<double>(val_losses[i]));
                }
            }
            if(train_losses != py::list())
            {
                TrainLosses->clear();
                for (int i = 0; i < py::len(train_losses); ++i)
                {
                    TrainLosses->push_back(boost::python::extract<double>(train_losses[i]));
                }
            }

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


                // ���������� ���������� ��� ������� ��������

                // ������� ���������� � ������ ���� PyObject

                // TODO ����� ����� ��������� �� ������� ������ (���� ��� ������������)
                py::list split_ratio;
                    split_ratio.append(0.7);      //SplitRatio[0]);
                    split_ratio.append(0.1);      //SplitRatio[1]);
                    split_ratio.append(0.2);      //SplitRatio[2]);


                //���������� ������� ���������� ( ����������� ���������)
                py::dict func_params;

                func_params["weights"]          =   py::str(Weights->c_str());
                func_params["epochs"]           =   py::object(*Epochs);
                func_params["architecture"]     =   py::str(ArchitectureName->c_str());
                func_params["split_ratio"]      =   split_ratio;
                func_params["save_splits"]      =   py::object(*SaveSplits);
                func_params["early_stop"]       =   *EarlyStop ? py::object(*EarlyStop) : py::object(false);
                func_params["preprocessing"]    =   py::object();
                func_params["saving_interval"]  =   py::object(*SavingInterval);
                func_params["save_best_only"]   =   py::object(*SaveBestOnly);
                func_params["augmentation"]     =   py::dict();
                func_params["save_predicted"]   =   py::object(*SavePredicted);
                func_params["visualize"]        =   (*Visualize == 0) ? py::object(false)     // ���� ����� 0  - �� �������� false
                                                    :( (*Visualize == -1) ? py::str("All")    // ���� ����� -1 - �� �������� ������ "All"
                                                    :  py::object(*Visualize) );              // ����� �������� ����� ������ ����
                func_params["paint_gt"]         =   py::object(*PaintGt);


                // ������ ����� ������ ��� ��������
                py::list train_data_dirs;
                for(int i = 0; i < TrainDataDir->size(); i++)
                {
                    train_data_dirs.append(py::str(TrainDataDir->at(i).c_str()));
                }

                // ����������� ���������
                py::tuple args_tuple = py::make_tuple(train_data_dirs,
                                                      DatasetType->c_str(),
                                                      WorkingDir->c_str(),
                                                      Config->c_str());

                //������ ��������, ������ ������� ������ ������������ �������� � �����
                py::object retval = IntegrationInterfaceInstance.attr("detection_train")
                                                                        (args_tuple,
                                                                         func_params);

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



