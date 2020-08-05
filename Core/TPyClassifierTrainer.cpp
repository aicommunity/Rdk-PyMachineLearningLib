#ifndef RDK_TPyClassifierTrainerCPP
#define RDK_TPyClassifierTrainerCPP

#include "TPyClassifierTrainer.h"
#include <iostream>

namespace RDK {

// ������
// --------------------------
// ������������ � �����������
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
  SaveBestOnly("SaveBestOnly",this)
{
}

TPyClassifierTrainer::~TPyClassifierTrainer(void)
{
}
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
TPyClassifierTrainer* TPyClassifierTrainer::New(void)
{
 return new TPyClassifierTrainer;
}
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
bool TPyClassifierTrainer::APythonInitialize(void)
{
    return true;
}

// �������������� �������� �� ��������� � ����� �������� �����
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

    return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TPyClassifierTrainer::APyBuild(void)
{

    return true;
}

// ����� �������� ����� ��� ������ ��������
bool TPyClassifierTrainer::APyReset(void)
{

    return true;
}

// ��������� ������ ����� �������
bool TPyClassifierTrainer::APyCalculate(void)
{
    // �������� �� ������� ���������
    if(!CheckInputParameters())
        return true;

    try
    {
        // ������� ���������� � ��� PyObject
        py::str data_dir(TrainDataDir->c_str());
        py::str working_dir(WorkingDir->c_str());
        py::str architecture(ArchitectureName->c_str());
        py::str dataset_name(DatasetName->c_str());


        py::list split_ratio;
            split_ratio.append(SplitRatio[0]);
            split_ratio.append(SplitRatio[1]);
            split_ratio.append(SplitRatio[2]);


        py::object save_splits(SaveSplits.v);
        py::object copy_images(CopySplittedImages.v);
        py::object test_equal_to_val(TestEqualVal.v);

        py::tuple image_size = py::make_tuple(ImageSize[0],ImageSize[1],ImageSize[2]);

        py::object epochs(Epochs.v);

        py::object learning_rate(LearningRate.v);

        py::list batch_sizes;
            batch_sizes.append(BatchSizes[0]);
            batch_sizes.append(BatchSizes[1]);
            batch_sizes.append(BatchSizes[2]);

        py::str weights(Weights->c_str());



        py::object layers_to_be_trained;

        if(LayersToBeTrained.v)
            layers_to_be_trained = py::object(LayersToBeTrained.v);
        else
            layers_to_be_trained = py::str("default");


        py::object classes;
        if(!Classes.empty())
        {
            split_ratio.append(Classes[0]);
            split_ratio.append(Classes[1]);
            split_ratio.append(Classes[2]);
        }

        py::object early_stop;

        if(EarlyStop.v)
            early_stop = py::object(EarlyStop.v);
        else
            early_stop = py::object(false);


        py::object preprocessing;

        py::object saving_interval(SavingInterval.v);

        py::object save_best_only(SaveBestOnly.v);

        py::dict online_augmentation = {};

        //TODO ��� ������� ������� ��� �������� �� �� �����, �����������
        // � ������ � ��� TPyClassifierTrainer dont exist in pipeline
        py::object retval = IntegrationInterfaceInstance.attr("training_interface")
                                                            (data_dir,
                                                             working_dir,
                                                             architecture,
                                                             dataset_name,
                                                             split_ratio,
                                                             save_splits,
                                                             copy_images,
                                                             test_equal_to_val,
                                                             image_size,
                                                             epochs,
                                                             learning_rate,
                                                             batch_sizes,
                                                             weights,
                                                             layers_to_be_trained,
                                                             classes,
                                                             early_stop,
                                                             preprocessing,
                                                             saving_interval,
                                                             save_best_only,
                                                             online_augmentation
                                                             );
    }
    catch (py::error_already_set const &)
    {
     std::string perrorStr = parse_python_exception();
     LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyClassifierTrainer error: ")+perrorStr);
    }
    catch(...)
    {
     LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception"));
    }
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
    //�������� ������ ��� �������� �� �����.�������� � ����.
    return true;

}

// --------------------------

}
#endif



