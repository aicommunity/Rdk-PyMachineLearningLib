#ifndef RDK_TPyClassifierTrainerH
#define RDK_TPyClassifierTrainerH

#include "TPyComponent.h"




namespace RDK {

class TPyClassifierTrainer: public TPyComponent
{
public: // ��������  
/// ����� � ������������� ��� ��������
/*
����� ������������ ����� �������� ����� ������ � �������
    main_directory/
    ...class_a/
    ......a_image_1.jpg
    ......a_image_2.jpg
    ...class_b/
    ......b_image_1.jpg
    ......b_image_2.jpg
*/
/// ��� ��� ����� � ����������� ���������� train, val, test
/// ��� ���� ���� - ������, �� ��� ����� .txt ����� �� ��������� "����_��_�������� �����",
/// ���������� ��������� ��� ������� train, val, tes
ULProperty<std::string, TPyClassifierTrainer> TrainDataDir;

/// ����������, ���� ���������� ����������������� �������, ����� ���� �������, �������, ������� ������
ULProperty<std::string, TPyClassifierTrainer> WorkingDir;

/// ��� �����������
/// ��������: SqueezeNet, MobileNet, MobileNetV2, InceptionV3, VGG16, ResNet50,
/// ResNet101, DenseNet121, DenseNet169, NASNetMobile, NASNetLarge
ULProperty<std::string, TPyClassifierTrainer> ArchitectureName;

/// ��� ������ ������ ��� ���������� � ����� ������� � �������� �����
ULProperty<std::string, TPyClassifierTrainer> DatasetName;

/// [percent_train, percent_val, percent_test] - ��������� ��������� �������,
/// ���� ������������ �������� ���������� (��� ��������� �� train val test, �� ��������� 70, 20, 10)
ULProperty<std::vector<int>, TPyClassifierTrainer> SplitRatio;

/// ��������� �� ��������� ����� ���������.
ULProperty<bool, TPyClassifierTrainer> SaveSplits;

/// ���������� �� � ������� ���������� �������� �������� �� train val test
/// �������� ��������������� ���������� �� ��������� ������
ULProperty<bool, TPyClassifierTrainer> CopySplittedImages;

/// ����� �� ������������ ������������� ������� ��������.
/// � ������ ���� ��������� ����� 70, 20, 10 � TestEqualVal=True, �� 10% ������� �� ����� ������������.
ULProperty<bool, TPyClassifierTrainer> TestEqualVal;

/// ��������� ����������� (width, height, channels)
ULProperty<std::vector<int>, TPyClassifierTrainer> ImageSize;

/// ���-�� ���� ��� ��������
ULProperty<int, TPyClassifierTrainer> Epochs;

/// Learning Rate
ULProperty<float, TPyClassifierTrainer> LearningRate;

/// ������� ���� batch-��, ��� train, val � test
ULProperty<std::vector<int>, TPyClassifierTrainer> BatchSizes;

/// ��������� ����. �� ��������� "imagenet" ����� random ��� ���� �� ���������� �����
ULProperty<std::string, TPyClassifierTrainer> Weights;

/// ���������� ����������� �����. N ���������
ULProperty<int, TPyClassifierTrainer> LayersToBeTrained;

/// ������, �����������(������), ���� ������ �� �� ���� ������� ��������
ULProperty<std::vector<std::string>, TPyClassifierTrainer> Classes;

/// �����, ������ ��������� patience
/// (���������� ����, ������� ����� ���� ��������� ��� ��������� �������� ������� ������, �� ��������� ��������)
ULProperty<int, TPyClassifierTrainer> EarlyStop;

/// �������� ���������� ����� (� ������ �������� ��� tf 1.* - n ����) (� ������ �������� tf 2.* - n ������)
ULProperty<int, TPyClassifierTrainer> SavingInterval;

/// C�������� �� ������ ������ ������ �� ��������� val_loss
ULProperty<bool, TPyClassifierTrainer> SaveBestOnly;

/// ���� ��������� ��������
// �������� �������� ��� StartTraining=true � TrainingInProgress=false
// ��� ������� �������� ������ ���� ���������� false
ULProperty<bool, TPyClassifierTrainer> StartTraining;

/// ���� ��������� ��������
// ������������� �������� � ������ ����, ���� ������ ������ ����� �����, ����� ����������� �����
ULProperty<bool, TPyClassifierTrainer> StopTraining;

/// ���� ��������� �������� (�������� ��� ������)
ULProperty<bool, TPyClassifierTrainer> StopNow;


/// ������ ��������
// 0 - ������ �� ����������
// 1 - ��������
// 2 - ������������ ����� ��������
// 3 -
ULProperty<int, TPyClassifierTrainer, ptPubState> TrainingStatus;

/// ������� �����
ULProperty<int, TPyClassifierTrainer, ptPubState> Epoch;

/// ������� ������ �� ������������� ������
ULProperty<float, TPyClassifierTrainer, ptPubState> TrainLoss;

/// ������� �������� �� ������������� ������
ULProperty<float, TPyClassifierTrainer, ptPubState> TrainAcc;

/// ������� ������ �� ������������� ������
ULProperty<float, TPyClassifierTrainer, ptPubState> ValLoss;

/// ������� �������� �� ������������� ������
ULProperty<float, TPyClassifierTrainer, ptPubState> ValAcc;

/// ������� ��������
ULProperty<float, TPyClassifierTrainer, ptPubState> Progress;



protected: // ��������� ����������



public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyClassifierTrainer(void);
virtual ~TPyClassifierTrainer(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TPyClassifierTrainer* New(void);
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
bool APythonInitialize(void);

// �������������� �������� �� ��������� � ����� �������� �����
virtual bool APyDefault(void);

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool APyBuild(void);

// ����� �������� ����� ��� ������ ��������
virtual bool APyReset(void);

// ��������� ������ ����� �������
virtual bool APyCalculate(void);


// ��������� ������� ��������� ����� �������� python-�������
bool CheckInputParameters();
// --------------------------
};


}

#endif



