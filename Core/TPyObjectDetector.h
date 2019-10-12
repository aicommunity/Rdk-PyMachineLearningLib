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
UPropertyInputData<UBitmap, TPyObjectDetector> InputImage;

/// �������� ���������� �����������
UPropertyOutputData<UBitmap, TPyObjectDetector> DebugImage;

/// ����� �����, ������������ �������� ������, �� ������� ���������� ����
/// ubmRGB24=3 - ������� �����������
/// umbY8=400 - �����-����� �����������
ULProperty<int,TPyObjectDetector, ptPubParameter> ImageColorModel;

///�������� �� ���������� ��
ULProperty<bool,TPyObjectDetector, ptPubParameter> UseRelativeCoords;

///�������� �� ���������� ��
ULProperty<bool,TPyObjectDetector, ptPubParameter> UseDebugImage;

/// �������� �������. ���������� ����� �� ����� ��������
/// ������ �������:
/// ������ �� ���������� ��������
/// ������ 4+1=Left; Top; Right; Bottom; ClassNumber
UPropertyOutputData<MDMatrix<double>, TPyObjectDetector, ptOutput | ptPubState> OutputObjects;

/// �������� ������� ������ ��������������� � ��������� ������������
UPropertyOutputData<MDMatrix<double>, TPyObjectDetector, ptOutput | ptPubState> OutputRects;

/// �������� ������� ��������������� �������
UPropertyOutputData<MDMatrix<int>, TPyObjectDetector, ptOutput | ptPubState> OutputClasses;

/// �������� ������� ������ ��������������
UPropertyOutputData<MDMatrix<double>, TPyObjectDetector, ptOutput | ptPubState> OutputReliability;

protected: // ��������� ����������
UGraphics Graph;

UBitmap ProcessedBmp;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyObjectDetector(void);
virtual ~TPyObjectDetector(void);
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:

// �������������� �������� �� ��������� � ����� �������� �����
virtual bool APyDefault(void);
virtual bool APyDefault2(void);

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool APyBuild(void);
virtual bool APyBuild2(void);

// ����� �������� ����� ��� ������ ��������
virtual bool APyReset(void);
virtual bool APyReset2(void);

// ��������� ������ ����� �������
virtual bool APyCalculate(void);
virtual bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
// --------------------------
};


}

#endif



