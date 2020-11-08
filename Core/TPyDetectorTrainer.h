#ifndef RDK_TPyDetectorTrainerH
#define RDK_TPyDetectorTrainerH

#include "TPyComponent.h"




namespace RDK {

class TPyDetectorTrainer: public TPyComponent
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
ULProperty<std::string, TPyDetectorTrainer> TrainDataDir;

/// ����������, ���� ���������� ����������������� �������, ����� ���� �������, �������, ������� ������
ULProperty<std::string, TPyDetectorTrainer> WorkingDir;

/// ��� �����������
/// ��������: SqueezeNet, MobileNet, MobileNetV2, InceptionV3, VGG16, ResNet50,
/// ResNet101, DenseNet121, DenseNet169, NASNetMobile, NASNetLarge
ULProperty<std::string, TPyDetectorTrainer> ArchitectureName;

/// ��� ������ ������ ��� ���������� � ����� ������� � �������� �����
ULProperty<std::string, TPyDetectorTrainer> DatasetName;

/// [percent_train, percent_val, percent_test] - ��������� ��������� �������,
/// ���� ������������ �������� ���������� (��� ��������� �� train val test, �� ��������� 70, 20, 10)
ULProperty<std::vector<int>, TPyDetectorTrainer> SplitRatio;

/// ��������� �� ��������� ����� ���������.
ULProperty<bool, TPyDetectorTrainer> SaveSplits;

/// ���������� �� � ������� ���������� �������� �������� �� train val test
/// �������� ��������������� ���������� �� ��������� ������
ULProperty<bool, TPyDetectorTrainer> CopySplittedImages;

/// ����� �� ������������ ������������� ������� ��������.
/// � ������ ���� ��������� ����� 70, 20, 10 � TestEqualVal=True, �� 10% ������� �� ����� ������������.
ULProperty<bool, TPyDetectorTrainer> TestEqualVal;

/// ��������� ����������� (width, height, channels)
ULProperty<std::vector<int>, TPyDetectorTrainer> ImageSize;

/// ���-�� ���� ��� ��������
ULProperty<int, TPyDetectorTrainer> Epochs;

/// Learning Rate
ULProperty<float, TPyDetectorTrainer> LearningRate;

/// ������� ���� batch-��, ��� train, val � test
ULProperty<std::vector<int>, TPyDetectorTrainer> BatchSizes;

/// ��������� ����. �� ��������� "imagenet" ����� random ��� ���� �� ���������� �����
ULProperty<std::string, TPyDetectorTrainer> Weights;

/// ���������� ����������� �����. N ���������
ULProperty<int, TPyDetectorTrainer> LayersToBeTrained;

/// ������, �����������(������), ���� ������ �� �� ���� ������� ��������
ULProperty<std::vector<std::string>, TPyDetectorTrainer> Classes;

/// �����, ������ ��������� patience
/// (���������� ����, ������� ����� ���� ��������� ��� ��������� �������� ������� ������, �� ��������� ��������)
ULProperty<int, TPyDetectorTrainer> EarlyStop;

/// �������� ���������� ����� (� ������ �������� ��� tf 1.* - n ����) (� ������ �������� tf 2.* - n ������)
ULProperty<int, TPyDetectorTrainer> SavingInterval;

/// C�������� �� ������ ������ ������ �� ��������� val_loss
ULProperty<bool, TPyDetectorTrainer> SaveBestOnly;

/// ���� ��������� ��������
// �������� �������� ��� StartTraining=true � TrainingInProgress=false
// ��� ������� �������� ������ ���� ���������� false
ULProperty<bool, TPyDetectorTrainer> StartTraining;

/// ���� ��������� ��������
// ������������� �������� � ������ ����, ���� ������ ������ ����� �����, ����� ����������� �����
ULProperty<bool, TPyDetectorTrainer> StopTraining;

/// ���� ��������� �������� (�������� ��� ������)
ULProperty<bool, TPyDetectorTrainer> StopNow;


/// ������ ��������
// 0 - ������ �� ����������
// 1 - ��������
// 2 - ������������ ����� ��������
// 3 -
ULProperty<int, TPyDetectorTrainer, ptPubState> TrainingStatus;

/// ������� �����
ULProperty<int, TPyDetectorTrainer, ptPubState> Epoch;

/// ������� ������ �� ������������� ������
ULProperty<float, TPyDetectorTrainer, ptPubState> TrainLoss;

/// ������� �������� �� ������������� ������
ULProperty<float, TPyDetectorTrainer, ptPubState> TrainAcc;

/// ������� ������ �� ������������� ������
ULProperty<float, TPyDetectorTrainer, ptPubState> ValLoss;

/// ������� �������� �� ������������� ������
ULProperty<float, TPyDetectorTrainer, ptPubState> ValAcc;

/// ������� ��������
ULProperty<float, TPyDetectorTrainer, ptPubState> Progress;



protected: // ��������� ����������



public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyDetectorTrainer(void);
virtual ~TPyDetectorTrainer(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TPyDetectorTrainer* New(void);
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
virtual bool ACalculate(void);


// ��������� ������� ��������� ����� �������� python-�������
bool CheckInputParameters();
// --------------------------
};


}

#endif



