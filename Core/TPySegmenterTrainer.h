#ifndef RDK_TPySegmenterTrainerH
#define RDK_TPySegmenterTrainerH

#include "TPyBaseTrainer.h"




namespace RDK {

class TPySegmenterTrainer: public TPyBaseTrainer
{
public: // ��������  

    // � TrainDataDir 2 ����:
    //      "basedir_ann" - ���� � ���������� ����
    //      "basedir_img" - ���� � ������������ ����

/// �������� ��������� ����
ULProperty<std::string, TPySegmenterTrainer> DatasetName;

/// ������ ����������� (��������� ������ ��� unet)
ULProperty<std::string, TPySegmenterTrainer> Model;

/// ������, �� ������� ����� ��������� �����������
ULProperty<int, TPySegmenterTrainer> TrainingSize;

/// �������� ������ (�������������� ������)
ULProperty<std::vector<int>, TPySegmenterTrainer> OutputSize;

/// ���������� �������
ULProperty<int, TPySegmenterTrainer> NumClasses;

/// �������� �������
ULProperty<std::vector<std::string>, TPySegmenterTrainer> Classes;

/// ���� �� ��������� ���� �� train val test
ULProperty<bool, TPySegmenterTrainer> SplitBase;

/// ���� � ���������� ������ (���� ���, ��������� "")
ULProperty<std::string, TPySegmenterTrainer> TrainList;

/// ���� � �������������� ������ (���� ���, ��������� "")
ULProperty<std::string, TPySegmenterTrainer> ValList;

/// ���� � ��������� ������ (���� ���, ��������� "")
ULProperty<std::string, TPySegmenterTrainer> TestList;

/// ���������� ����������� � �����
ULProperty<int, TPySegmenterTrainer> BatchSize;

/// ���������� ������ � ����� (TODO ������ � 3-�� ����������?)
ULProperty<int, TPySegmenterTrainer> BatchesInEpoch;

// ���������� ���������
/// ������ �� �������� ������������
ULProperty<float, TPyBaseTrainer, ptPubState> TestLoss;

/// �������� �� �������� ������������
ULProperty<float, TPyBaseTrainer, ptPubState> TestAcc;

/// ������ �� ������������� ������������
ULProperty<float, TPyBaseTrainer, ptPubState> ValLoss;

/// �������� �� ������������� ������������
ULProperty<float, TPyBaseTrainer, ptPubState> ValAcc;

/// �������� ������ ������
ULProperty<float, TPyBaseTrainer, ptPubState> FPS;

protected: // ��������� ����������



public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPySegmenterTrainer(void);
virtual ~TPySegmenterTrainer(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TPySegmenterTrainer* New(void);
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



