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
: Config("Config",this),
  DatasetType("DatasetType",this),
  DatasetName("DatasetName",this),
  InputRes("InputRes",this),
  Classes("Classes",this),
  BatchSize("BatchSize",this),
  BatchesInEpoch("BatchesInEpoch",this),
  TrainLoss("TestLoss",this),
  TrainAcc("TestAcc",this),
  ValLoss("ValLoss",this),
  ValAcc("ValAcc",this)
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
    PythonModuleName="segmentation_interface";
    PythonClassName="SegmentationInterface";

    // ����� ��������� ��� ���� ��������
    TrainDataDir = {""};
    WorkingDir = "";
    ArchitectureName= "";
    SplitRatio = {70, 15, 15};
    SaveSplits = false;
    Epochs = 5;
    Weights = "";
    EarlyStop = 0;
    SavingInterval = 1;
    SaveBestOnly = false;

    // ������������� ��������� ��� �������� �������������
    Config = "";
    DatasetType = "";
    DatasetName = "";
    InputRes = {1024,512,3};
    Classes = {"road", "sidewalk", "building", "wall", "fence", "pole",
               "traffic light", "traffic sign", "vegetation", "terrain", "sky", "person",
               "rider", "car", "truck", "bus", "train", "motocycle", "bicycle"};
    BatchSize = 12;
    BatchesInEpoch = 5;


    TrainingStatus = 0;
    StartTraining = false;
    StopTraining = false;

    Epoch    = 0;
    Progress = 0.0;
    TrainLoss = 0.0;
    TrainAcc  = 0.0;
    ValLoss  = 0.0;
    ValAcc   = 0.0;

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

    if(WorkingDir->empty())
    {
        WorkingDir = Environment->GetCurrentDataDir()+"Results/";
    }

    gil_lock lock;
    try
    {   //��������� ������ ������� ������ (�������� GIL ����) ��� ���������� ������� �������


        // �������� ������� ����������
        py::object train_status = IntegrationInterfaceInstance->attr("train_status")();
        TrainingStatus = boost::python::extract< int >(train_status);

        ThreadIsAlive = boost::python::extract<bool>(IntegrationInterfaceInstance->attr("get_thread_is_alive")());

        // ������ �� ����� �������� (�������� � �������� ������)
        // ���� ��������� ��������� stop_now ��� stop_training
        if(TrainingStatus == -1)
        {
            //����� �� ������ ����������� �����
            StartTraining = false;

            py::object except_string = IntegrationInterfaceInstance->attr("get_error_string")();

            std::string PyExceptionString = boost::python::extract< std::string >(except_string);

            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Exception or proper stop: ") + PyExceptionString);

            // ����� �������
            TrainingStatus = 0;
            py::object res = IntegrationInterfaceInstance->attr("set_training_status_to_null")();

            // ����� ���������� ���������
            Epoch    = 0;
            Progress = 0.0;
            TrainLoss = 0.0;
            TrainAcc  = 0.0;
            ValLoss  = 0.0;
            ValAcc   = 0.0;
        }
        // �������� ���������� ��������. ����� ��������� � ���������� ������������ � 0
        // �������� � ���������� ������ � 0
        if(TrainingStatus == 3)
        {
            //����� �� ������ ����������� �����
            StartTraining = false;

            LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Training completed correctly"));

            // ����� �������
            TrainingStatus = 0;
            py::object res = IntegrationInterfaceInstance->attr("set_training_status_to_null")();

            // ����� ���������� ���������
            Epoch    = 0;
            Progress = 0.0;
            TrainLoss = 0.0;
            TrainAcc  = 0.0;
            ValLoss  = 0.0;
            ValAcc   = 0.0;
        }
        // ���� ��������/������������ ����, ���������� �������
        if(TrainingStatus == 1 || TrainingStatus == 2)
        {
            //����� �� ������ ����������� �����
            StartTraining = false;

            //������ �������� ��� ��������� ���������� � ��������� ����������
            Epoch       = boost::python::extract< int >  (IntegrationInterfaceInstance->attr("get_epoch")());
            Progress    = boost::python::extract< float >(IntegrationInterfaceInstance->attr("get_progess")());

            TrainAcc    = boost::python::extract< float >(IntegrationInterfaceInstance->attr("get_train_acc")());
            TrainLoss   = boost::python::extract< float >(IntegrationInterfaceInstance->attr("get_train_loss")());
            ValAcc      = boost::python::extract< float >(IntegrationInterfaceInstance->attr("get_val_acc")());
            ValLoss     = boost::python::extract< float >(IntegrationInterfaceInstance->attr("get_val_loss")());


            //������������� �������� ���� ������ ��� �������, ���� ���������
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

                    StartTraining = false;
                    return true;
                }

                // �������� �� ������������ ������� ����������
                if(!CheckInputParameters())
                {
                    StartTraining = false;
                    return true;
                }

                LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Training started"));

                // ��������� �������� ������
                StopTraining = StopNow = false;

                // ���������� ���������� ��� ������� ��������

                // ������� ���������� � ������ ���� PyObject
                py::list split_ratio;
                    split_ratio.append(SplitRatio[0]);
                    split_ratio.append(SplitRatio[1]);
                    split_ratio.append(SplitRatio[2]);

                py::list classes;
                if(!Classes.empty())
                {
                    for(int i = 0; i < Classes.size(); i++)
                    {
                        classes.append(py::str(Classes[i].c_str()));
                    }
                }

                py::list input_res;
                if(!InputRes.empty())
                {
                    for(int i = 0; i < InputRes.size(); i++)
                    {
                        input_res.append(InputRes[i]);
                    }
                }

                //���������� ������� ���������� ( ����������� ���������)
                py::dict func_params;

                func_params["working_dir"]      =   py::str(WorkingDir->c_str());
                func_params["architecture"]     =   py::str(ArchitectureName->c_str());
                func_params["dataset_name"]     =   py::str(DatasetName->c_str());
                func_params["weights"]          =   py::str(Weights->c_str());
                func_params["split_ratio"]      =   split_ratio;
                func_params["input_res"]        =   input_res;
                func_params["classes"]          =   classes;
                func_params["batch_size"]       =   py::object(*BatchSize);
                func_params["epochs"]           =   py::object(*Epochs);
                func_params["batches_in_epoch"] =   py::object(*BatchesInEpoch);
                func_params["saving_interval"]  =   py::object(*SavingInterval);
                func_params["save_best_only"]   =   py::object(*SaveBestOnly);


                // ������ ����� ������ ��� ��������
                /*
                py::object data_path;

                if(TrainDataDir.size() == 1)
                {
                    data_path = py::str(TrainDataDir->at(0).c_str());
                }
                if(TrainDataDir.size() > 1)
                {
                    py::list train_data_dirs;
                    for(int i = 0; i < TrainDataDir->size(); i++)
                    {
                        train_data_dirs.append(py::str(TrainDataDir->at(i).c_str()));
                    }
                    data_path = train_data_dirs;
                }
                */
                // ����������� ���������
                py::tuple args_tuple = py::make_tuple(py::str(Config->c_str()),
                                                      py::str(TrainDataDir->at(0).c_str()),
                                                      py::str(DatasetType->c_str()));

                //������ ��������, ������ ������� ������ ������� �������� ����������� � ��������� �����
                py::object retval = IntegrationInterfaceInstance->attr("segmentation_train")
                                                                        (args_tuple,
                                                                         func_params);

                // �������� �� �������������� ������
                // ���� ����� ���������� ������� segmentation_train() ����� ��������� TrainingStatus �� -1
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
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPySegmenterTrainer error: ")+perrorStr);
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


bool TPySegmenterTrainer::CheckInputParameters()
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

    if(SplitRatio.size()!=3)
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("SplitRatio must have 3 values!"));
        return false;
    }

    if(DatasetName->empty())
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("DatasetName parameter is empty!"));
        return false;
    }


    //TODO �������� ����� ��� �������� �� �����.�������� � ����.
    //TODO �������� �� ���� ������������� � �.�.
    return true;
}

// --------------------------

}
#endif



