#ifndef TPyObjectDetectorYoloExH
#define RDK_TPyObjectDetectorYoloExH

#include "TPyObjectDetector.h"

namespace RDK {

class TPyObjectDetectorYoloEx: public TPyObjectDetector
{
public: // ��������
///���� � ������ ����
ULProperty<std::string,TPyObjectDetectorYoloEx, ptPubParameter> ModelPathYOLO;

///���� � ����� � ������� (�������� ������ ���� ���������� ��������)
ULProperty<std::string,TPyObjectDetectorYoloEx, ptPubParameter> AnchorsPathYOLO;

///���� � ����� � �������� ������� ��� ������������ ����
ULProperty<std::string,TPyObjectDetectorYoloEx, ptPubParameter> ClassesPathYOLO;

///������ ������� �������
ULProperty<std::vector<std::string>,TPyObjectDetectorYoloEx, ptPubParameter> TargetClassesYOLO;

///��������� ������ ������� �� ���������������� �����
ULProperty<bool,TPyObjectDetectorYoloEx, ptPubParameter> LoadTargetClassesYOLO;

ULProperty<int,TPyObjectDetectorYoloEx, ptPubParameter> NumTargetClassesYOLO;
ULProperty<int,TPyObjectDetectorYoloEx, ptPubParameter>  NumChangeClassesYOLO;

///������ ����� ������� (�������� ������ ������� � ����� ClassesPathYOLO �� ���� �����)
///��������!!! ����� ������� ������ ��������������� ����� ������� � ClassesPathYOLO
ULProperty<std::vector<std::string>,TPyObjectDetectorYoloEx, ptPubParameter> ChangeClassesYOLO;

protected: // ��������� ����������

std::vector<std::string> ClassedList;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyObjectDetectorYoloEx(void);
virtual ~TPyObjectDetectorYoloEx(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
bool SetNumTargetClassesYOLO(const int& num);

bool SetNumChangeClassesYOLO(const int& num);
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TPyObjectDetectorYoloEx* New(void);
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
bool APythonInitialize(void);

// �������������� �������� �� ��������� � ����� �������� �����
virtual bool APyDefault2(void);

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool APyBuild2(void);

// ����� �������� ����� ��� ������ ��������
virtual bool APyReset2(void);

// ��������� �����������
virtual bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
// --------------------------
};


}

#endif



