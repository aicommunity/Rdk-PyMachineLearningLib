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


/// ���� � ����������������� �����
ULProperty<std::string, TPySegmenterTrainer> Config;

/// ��� ���� �� ��������� ��������: "txt_data", "split_data", "not_split_data".
ULProperty<std::string, TPySegmenterTrainer> DatasetType;

/// ��� ������ ������ ��� ���������� � ����� ������� � �������� ������� �������������
ULProperty<std::string, TPySegmenterTrainer> DatasetName;

/// ������, �� ������� ����� ��������� �����������, ������: [height,width,channels].
ULProperty<std::vector<int>, TPySegmenterTrainer> InputRes;

/// �������� �������
ULProperty<std::vector<std::string>, TPySegmenterTrainer> Classes;

/// ���������� ����������� � �����
ULProperty<int, TPySegmenterTrainer> BatchSize;

/// ���������� ������ � �����
ULProperty<int, TPySegmenterTrainer> BatchesInEpoch;


// ���������� ���������
/// ������ �� ��������� ������������
ULProperty<float, TPyBaseTrainer, ptPubState> TrainLoss;

/// �������� �� ��������� ������������
ULProperty<float, TPyBaseTrainer, ptPubState> TrainAcc;

/// ������ �� ������������� ������������
ULProperty<float, TPyBaseTrainer, ptPubState> ValLoss;

/// �������� �� ������������� ������������
ULProperty<float, TPyBaseTrainer, ptPubState> ValAcc;

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



