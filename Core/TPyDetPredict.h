#ifndef RDK_TPyDetPredictH
#define RDK_TPyDetPredictH

#include "TPyComponent.h"


namespace RDK {

class TPyDetPredict : public TPyComponent
{
public: // ��������  

/// ����������, ���� ���������� ��������� ������������
ULProperty<std::string, TPyDetPredict> WorkingDir;

/// ���������� � ����������� ��� ������������
ULProperty<std::string, TPyDetPredict> ImagesDir;

/// ���� � ����� ��������� ����
ULProperty<std::string, TPyDetPredict> WeightPath;

/// ���� � ������������ ��������� ����
ULProperty<std::string, TPyDetPredict> ConfigPath;

/// ���� ��������� ������������
ULProperty<bool, TPyDetPredict> StopNow;

/// ������� ��������� ������ ������������
// true  - ����� �����������
// false - ����� �� �����������
ULProperty<bool, TPyDetPredict, ptPubState> ThreadIsAlive;

// ����������� ��������� ��������� (� ��������������)
/// ���� ��������� ��������
// �������� �������� ��� StartTraining=true � PredictStatus=0
ULProperty<bool, TPyDetPredict> StartPredict;

/// ������ ������������
// -1 - ������ (���������� ������ ������ ������) ���� �������� ��������������� ���������� (stop_training, stop_now)).
// ����� ��������� � ���������� ������������ � 0
// 0 - ������ �� ���������� (����� ��������� ������������)
// 1 - ���� ������������
// 2 - ������������ ������� ���������
ULProperty<int, TPyDetPredict, ptPubState> PredictStatus;

protected: // ��������� ����������


public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyDetPredict(void);
virtual ~TPyDetPredict(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TPyDetPredict* New(void);
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
virtual bool CheckInputParameters();
// --------------------------
};


}

#endif



