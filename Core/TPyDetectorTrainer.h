#ifndef RDK_TPyDetectorTrainerH
#define RDK_TPyDetectorTrainerH

#include "TPyBaseTrainer.h"




namespace RDK {

class TPyDetectorTrainer: public TPyBaseTrainer
{
public: // ��������  

/// ��� ������ ������ �� ������ ������ ������ ��������� ��������: "xml_main_dirs", "xml_txt_splits", "xml_txt_main_lists", "xml_test_dirs", "xml_txt_test_splits"
ULProperty<std::string, TPyDetectorTrainer, ptPubParameter> DatasetType;

/// ������ � ������ � �������������� ����������� ��� �����������
ULProperty<std::string, TPyDetectorTrainer, ptPubParameter> Config;

/// ��������� �� �� ����� ������������ � �������� /pred ������� ����������
/// xml ����� ���������� ������������ ��������� �� �������� ������ ������
ULProperty<bool, TPyDetectorTrainer, ptPubParameter> SavePredicted;

/// ��������� � �������� /images ������� ���������� ����������� � �������������� ����������� ��������� ����.
/// ��������� ���������:
///     False   - �� ���������            - ����� 0
///     All     - ��������� ���           - ����� -1
///     n       - ��������� n ����������� - ������ ����� n
/// ���� ������ ����� n, �� ���������� ���������� n �������� �� ��������� ������ ������ 
ULProperty<int, TPyDetectorTrainer, ptPubParameter> Visualize;

/// �������� �� �������������� ����� ��� ��������� �� �������� ����������� ��� �������� ��������� visualize
/// (����� ��������� �������� ����� ������) ����� ���� ������� ��� ������� ���������� ����������� ��������,
/// ������ ������ ������ �������� ����������� ����������� ��� ������������
ULProperty<bool, TPyDetectorTrainer, ptPubParameter> PaintGt;

// ���������� ���������
/// ������ �������� ������� ������ ��� ���������� �����������
ULProperty<std::vector<std::string>, TPyDetectorTrainer, ptPubState> LossNames;

/// ������ �������� ������� ������ �� ������������� ������
ULProperty<std::vector<double>, TPyDetectorTrainer, ptPubState> TrainLosses;

/// ������ �������� ������� ������ �� ������������� ������
ULProperty<std::vector<double>, TPyDetectorTrainer, ptPubState> ValLosses;

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
//virtual bool APyReset(void);

// ��������� ������ ����� �������
virtual bool ACalculate(void);


// ��������� ������� ��������� ����� �������� python-�������
virtual bool CheckInputParameters();
// --------------------------
};


}

#endif



