#ifndef RDK_TPyBaseTrainerH
#define RDK_TPyBaseTrainerH

#include "TPyComponent.h"


namespace RDK {

/// ����������� ������� ����� ����������� �������� ��������� ����� ������ �����
class TPyBaseTrainer: public TPyComponent
{
public: // ��������

/// ����� � ������������� ��� �������� (����� ���� ��������� �����)
ULProperty<std::vector<std::string>, TPyBaseTrainer> TrainDataDir;

/// ����������, ���� ���������� ����������������� �������, ����� ���� �������, �������, ������� ������ � ����.
ULProperty<std::string, TPyBaseTrainer> WorkingDir;

/// ��� ����������� ��������� ����
ULProperty<std::string, TPyBaseTrainer> ArchitectureName;

/// ��������� ��������� ������� [percent_train, percent_val, percent_test]
/// ���� ������������ �������� ���������� (��� ��������� �� train val test, �� ��������� [0.7, 0.2, 0.1])
ULProperty<std::vector<int>, TPyBaseTrainer> SplitRatio;

/// ��������� �� ��������� ����� ���������
ULProperty<bool, TPyBaseTrainer> SaveSplits;

/// ���-�� ���� ��� ��������
ULProperty<int, TPyBaseTrainer> Epochs;

/// ��������� ���� (���� ���� ��������)
ULProperty<std::string, TPyBaseTrainer> Weights;

/// �����, ������ ��������� patience
/// (���������� ����, ������� ����� ���� ��������� ��� ��������� �������� ������� ������, �� ��������� ��������) 
ULProperty<int, TPyBaseTrainer> EarlyStop;

/// �������� ���������� ����� (� ������ �������� ��� tf 1.* - n ����) (� ������ �������� tf 2.* - n ������)
ULProperty<int, TPyBaseTrainer> SavingInterval;

/// C�������� �� ������ ������ ������ �� ��������� val_loss
ULProperty<bool, TPyBaseTrainer> SaveBestOnly;

/// ���� ��������� ��������
// �������� �������� ��� StartTraining=true � TrainingInProgress=0
ULProperty<bool, TPyBaseTrainer> StartTraining;

/// ���� ��������� ��������
// ������������� �������� � ������ ����, ���� ������ ������ ����� �����, ����� ������� �����������
ULProperty<bool, TPyBaseTrainer> StopTraining;

/// ���� ��������� �������� (�������� ��� ������)
ULProperty<bool, TPyBaseTrainer> StopNow;

/// ������ ��������
// -1 - ������ (���������� ������ ������ ������). ����� ��������� � ���������� ������������ � 0
// 0 - ������ �� ���������� (����� ��������� ��������)
// 1 - ��������
// 2 - ������������ ����� ��������
// 3 - �������� ���������� ������. ����� ��������� ������ ������������ � 0
//     (��������� ��������, ���� �������� ��������������� ���������� (stop_training, stop_now))
ULProperty<int, TPyBaseTrainer, ptPubState> TrainingStatus;

/// ������� �����
ULProperty<int, TPyBaseTrainer, ptPubState> Epoch;

/// ������� ��������
ULProperty<float, TPyBaseTrainer, ptPubState> Progress;


protected: // ��������� ����������



public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyBaseTrainer(void);
virtual ~TPyBaseTrainer(void);
// --------------------------

// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
virtual bool APythonInitialize(void);

// �������������� �������� �� ��������� � ����� �������� �����
virtual bool APyDefault(void);

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool APyBuild(void);

// ����� �������� ����� ��� ������ ��������
virtual bool APyReset(void) final;

// ��������� ������ ����� �������
virtual bool ACalculate(void);


// ��������� ������� ��������� ����� �������� python-�������
virtual bool CheckInputParameters() = 0;
// --------------------------
};


}

#endif



