#ifndef TPyObjectDetectorBasic
#define RDK_TPyObjectDetectorBasicH

#include "TPyComponent.h"

namespace RDK {

class TPyObjectDetectorBasic: public TPyComponent
{
public: // ��������
/// ������� �����������
UPropertyInputData<UBitmap, TPyObjectDetectorBasic, ptPubInput> InputImage;
UPropertyInputData<UBitmap, TPyObjectDetectorBasic, ptPubOutput> OutputImage;

/// ����� �����, ������������ �������� ������, �� ������� ���������� ����
/// ubmRGB24=3 - ������� �����������
/// umbY8=400 - �����-����� �����������
ULProperty<int,TPyObjectDetectorBasic, ptPubParameter> ImageColorModel;

/// ���������� ������� �������� (����� ������ ����� � �������
//ULProperty<int,TPyObjectDetectorBasic, ptPubParameter> NumClasses;model_path

///��� �������������: 1 - ����������, �����������, ����� ���� � ������, ������, ������� �������, �������� � ��������
///                   2 - ������������� ����� ���� � ������������� � ���� �����
//ULProperty<int,TPyObjectDetectorBasic, ptPubParameter> InitializationTypeYOLO;

///���� � ����� ������������ ����
//ULProperty<std::string,TPyObjectDetectorBasic, ptPubParameter> ConfigPathYOLO;
///���� � ����� ����� ����
//ULProperty<std::string,TPyObjectDetectorBasic, ptPubParameter> WeightsPathYOLO;



///���� � ������ ����
ULProperty<std::string,TPyObjectDetectorBasic, ptPubParameter> ModelPathYOLO;

///���� � ����� � ������� (�������� ������ ���� ���������� ��������)
ULProperty<std::string,TPyObjectDetectorBasic, ptPubParameter> AnchorsPathYOLO;

///���� � ����� � �������� ������� ��� ������������ ����
ULProperty<std::string,TPyObjectDetectorBasic, ptPubParameter> ClassesPathYOLO;

///������ ������� �������
ULProperty<std::vector<std::string>,TPyObjectDetectorBasic, ptPubParameter> TargetClassesYOLO;

///��������� ������ ������� �� ���������������� �����
ULProperty<bool,TPyObjectDetectorBasic, ptPubParameter> LoadTargetClassesYOLO;

///������ ����� ������� (�������� ������ ������� � ����� ClassesPathYOLO �� ���� �����)
///��������!!! ����� ������� ������ ��������������� ����� ������� � ClassesPathYOLO
ULProperty<std::vector<std::string>,TPyObjectDetectorBasic, ptPubParameter> ChangeClassesYOLO;

///������ ������� �� ���������
///ULProperty<std::map<int, int>,TPyObjectDetectorBasic, ptPubParameter> ClassIndicesExchange;


/// �������� ������� � �������� ��������
//UPropertyOutputData<std::vector<int>,TPyObjectDetectorBasic, ptPubOutput> OutputClasses;

/// �������� �������. ���������� �������� �� ����� ��������, ���������� ����� � ������� �� ����� �������
/// ������ �������:
/// ������ �� ���������� ��������
/// ������ 4+1=Left; Top; Right; Bottom; ClassNumber
UPropertyOutputData<MDMatrix<double>, TPyObjectDetectorBasic> OutputObjects;

protected: // ���������� ���������

UGraphics Graph;
UBitmap Canvas;

int NumTargetClassesYOLO;
int NumChangeClassesYOLO;

std::vector<std::string> ClassedList;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyObjectDetectorBasic(void);
virtual ~TPyObjectDetectorBasic(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
bool SetNumTargetClassesYOLO(const int& num);
const int& GetNumTargetClassesYOLO(void) const;

bool SetNumChangeClassesYOLO(const int& num);
const int& GetNumChangeClassesYOLO(void) const;
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TPyObjectDetectorBasic* New(void);
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
// --------------------------
};


}

#endif



