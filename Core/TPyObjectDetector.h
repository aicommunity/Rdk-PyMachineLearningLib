#ifndef RDK_TPyObjectDetectorH
#define RDK_TPyObjectDetectorH

#include "TPyComponent.h"

namespace RDK {

#define YOLOV2_INITTYPE 2
#define YOLOV3_INITTYPE 3

class TPyObjectDetector: public TPyComponent
{
public: // ��������
/// ������� �����������
UPropertyInputData<UBitmap, TPyObjectDetector, ptPubInput> InputImage;
UPropertyInputData<UBitmap, TPyObjectDetector, ptPubOutput> OutputImage;

/// ����� �����, ������������ �������� ������, �� ������� ���������� ����
/// ubmRGB24=3 - ������� �����������
/// umbY8=400 - �����-����� �����������
ULProperty<int,TPyObjectDetector, ptPubParameter> ImageColorModel;

/// ���������� ������� �������� (����� ������ ����� � �������
//ULProperty<int,TPyObjectDetector, ptPubParameter> NumClasses;model_path

///��� �������������: 2 - YOLOV2
///                   3 - YOLOV3
ULProperty<int,TPyObjectDetector, ptPubParameter> InitializationTypeYOLO;

///���� � ����� ������������ ����
ULProperty<std::string,TPyObjectDetector, ptPubParameter> ConfigPathYOLO;

///���� � ����� ����� ����
ULProperty<std::string,TPyObjectDetector, ptPubParameter> WeightsPathYOLO;



///���� � ������ ����
//ULProperty<std::string,TPyObjectDetector, ptPubParameter> ModelPathYOLO;

///���� � ����� � ������� (�������� ������ ���� ���������� ��������)
//ULProperty<std::string,TPyObjectDetector, ptPubParameter> AnchorsPathYOLO;

///���� � ����� � �������� ������� ��� ������������ ����
//ULProperty<std::string,TPyObjectDetector, ptPubParameter> ClassesPathYOLO;

///������ ������� �������
//ULProperty<std::vector<std::string>,TPyObjectDetector, ptPubParameter> TargetClassesYOLO;

///��������� ������ ������� �� ���������������� �����
//ULProperty<bool,TPyObjectDetector, ptPubParameter> LoadTargetClassesYOLO;

///������ ����� ������� (�������� ������ ������� � ����� ClassesPathYOLO �� ���� �����)
///��������!!! ����� ������� ������ ��������������� ����� ������� � ClassesPathYOLO
//ULProperty<std::vector<std::string>,TPyObjectDetector, ptPubParameter> ChangeClassesYOLO;

///������ ������� �� ���������
///ULProperty<std::map<int, int>,TPyObjectDetector, ptPubParameter> ClassIndicesExchange;


/// �������� ������� � �������� ��������
//UPropertyOutputData<std::vector<int>,TPyObjectDetector, ptPubOutput> OutputClasses;

/// �������� �������. ���������� �������� �� ����� ��������, ���������� ����� � ������� �� ����� �������
/// ������ �������:
/// ������ �� ���������� ��������
/// ������ 4+1=Left; Top; Right; Bottom; ClassNumber
UPropertyOutputData<MDMatrix<double>, TPyObjectDetector> OutputObjects;

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
TPyObjectDetector(void);
virtual ~TPyObjectDetector(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TPyObjectDetector* New(void);
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



