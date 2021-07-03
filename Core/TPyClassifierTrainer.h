#ifndef RDK_TPyClassifierTrainerH
#define RDK_TPyClassifierTrainerH

#include "TPyBaseTrainer.h"



namespace RDK {

class TPyClassifierTrainer: public TPyBaseTrainer
{
public: // ��������  

/// ��� ������ ������ ��� ���������� � ����� ������� � �������� �����
ULProperty<std::string, TPyClassifierTrainer> DatasetName;

/// ���������� �� � ������� ���������� �������� �������� �� train, val, test
/// �������� ��������������� ���������� �� ��������� ������
ULProperty<bool, TPyClassifierTrainer> CopySplittedImages;

/// ����� �� ������������ ������������� ������� ��������.
/// � ������ ���� ��������� ����� 70, 20, 10 � TestEqualVal=True, �� 10% ������� �� ����� ������������.
ULProperty<bool, TPyClassifierTrainer> TestEqualVal;

/// ��������� ����������� (width, height, channels)
ULProperty<std::vector<int>, TPyClassifierTrainer> ImageSize;

/// Learning Rate
ULProperty<float, TPyClassifierTrainer> LearningRate;

/// ������� ���� batch-��, ��� train, val � test
ULProperty<std::vector<int>, TPyClassifierTrainer> BatchSizes;

/// ���������� ����������� �����. N ���������
ULProperty<int, TPyClassifierTrainer> LayersToBeTrained;

/// ������, �����������(������), ���� ������ �� �� ���� ������� ��������
ULProperty<std::vector<std::string>, TPyClassifierTrainer> Classes;

/// ������� ������ �� ������������� ������
ULProperty<float, TPyBaseTrainer, ptPubState> TrainLoss;

/// ������� �������� �� ������������� ������
ULProperty<float, TPyBaseTrainer, ptPubState> TrainAcc;

/// ������� ������ �� ������������� ������
ULProperty<float, TPyBaseTrainer, ptPubState> ValLoss;

/// ������� �������� �� ������������� ������
ULProperty<float, TPyBaseTrainer, ptPubState> ValAcc;

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
//virtual bool APyReset(void);

// ��������� ������ ����� �������
virtual bool ACalculate(void);


// ��������� ������� ��������� ����� �������� python-�������
virtual bool CheckInputParameters();
// --------------------------
};


}

#endif



