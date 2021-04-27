#ifndef RDK_TPyPredictSortH
#define RDK_TPyPredictSortH

#include "TPyComponent.h"


namespace RDK {

class TPyPredictSort : public TPyComponent
{
public: // ��������  

/// ����������, ���� ���������� ��������� ������������
ULProperty<std::string, TPyPredictSort> WorkingDir;

/// ���������� � ����������� ��� ������������
ULProperty<std::string, TPyPredictSort> ImagesDir;

/// ���� � ����� ��������� ����
ULProperty<std::string, TPyPredictSort> WeightPath;

/// ���� � ������������ ��������� ����
ULProperty<std::string, TPyPredictSort> ConfigPath;

/// ����� �� ����������� ����������� �� ������� �� ������
ULProperty<bool, TPyPredictSort> SortImages;

/// ���� ��������� ������������
ULProperty<bool, TPyPredictSort> StopNow;

/// ������� ��������� ������ ������������
// true  - ����� �����������
// false - ����� �� �����������
ULProperty<bool, TPyPredictSort, ptPubState> ThreadIsAlive;

// ����������� ��������� ��������� (� ��������������)
/// ���� ��������� ��������
// �������� �������� ��� StartTraining=true � PredictStatus=0
ULProperty<bool, TPyPredictSort> StartPredict;

/// ���� ���������� ������������
ULProperty<bool, TPyPredictSort, ptPubState> PredictionEnded;

/// ������ ������������
// -1 - ������ (���������� ������ ������ ������) ���� �������� ��������������� ���������� (stop_training, stop_now)).
// ����� ��������� � ���������� ������������ � 0
// 0 - ������ �� ���������� (����� ��������� ������������)
// 1 - ���� ������������
// 2 - ������������ ������� ���������
ULProperty<int, TPyPredictSort, ptPubState> PredictStatus;

protected: // ��������� ����������


public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyPredictSort(void);
virtual ~TPyPredictSort(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TPyPredictSort* New(void);
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



