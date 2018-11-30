#ifndef RDK_TPyUBitmapClassifierH
#define RDK_TPyUBitmapClassifierH

#include "TPyComponent.h"

namespace RDK {

class TPyUBitmapClassifier: public TPyComponent
{
public: // ��������
/// ������� �����������
/// UPropertyInputData<UBitmap,TPyUBitmapClassifier> InputImage;

//������� ������� � ������� �� ������������
/// �������� ����������� (������������) ��� �������������
UPropertyInputData<std::vector<UBitmap>, TPyUBitmapClassifier, ptPubInput> InputImages;

/// ����� �����, ������������ �������� ������, �� ������� ���������� ����
/// ubmRGB24=3 - ������� �����������
/// umbY8=400 - �����-����� �����������
ULProperty<int,TPyUBitmapClassifier, ptPubParameter> ImageColorModel;

/// ���������� ������� �������� (����� ������ ����� � �������
ULProperty<int,TPyUBitmapClassifier, ptPubParameter> NumClasses;

/// �������� ������� � �������� ��������
UPropertyOutputData<std::vector<int>,TPyUBitmapClassifier, ptPubOutput> OutputClasses;

/// �������� �������. ���������� �������� �� ����� ��������, ���������� ����� � ������� �� ����� �������
/// ������ �������� - ����������� ������
UPropertyOutputData<MDMatrix<double>, TPyUBitmapClassifier> OutputConfidences;

protected: // ���������� ���������

UGraphics Graph;
UBitmap Canvas;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyUBitmapClassifier(void);
virtual ~TPyUBitmapClassifier(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TPyUBitmapClassifier* New(void);
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



