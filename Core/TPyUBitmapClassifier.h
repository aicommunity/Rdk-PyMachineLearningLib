#ifndef RDK_TPyUBitmapClassifierH
#define RDK_TPyUBitmapClassifierH

#include "TPyComponent.h"

namespace RDK {

class TPyUBitmapClassifier: public TPyComponent
{
public: // ��������
/// ������� �����������
/// ������������, ���� ��������� ��������� ���� InputImages
UPropertyInputData<UBitmap,TPyUBitmapClassifier> InputImage;

//������� ����������� � ������� �� ������������
/// �������� ����������� (������������) ��� �������������
UPropertyInputData<std::vector<UBitmap>, TPyUBitmapClassifier, ptPubInput> InputImages;

/// ����� �����, ������������ �������� ������, �� ������� ���������� ����
/// ubmRGB24=3 - ������� �����������
/// umbY8=400 - �����-����� �����������
ULProperty<int,TPyUBitmapClassifier, ptPubParameter> ImageColorModel;

/// ���������� ������� �������� (����� ������ ����� � �������)
ULProperty<int,TPyUBitmapClassifier, ptPubParameter> NumClasses;

///����� �����������: ���� ����� �� ��������� ������ �����������, �� �� ������������ � 0, ��� ������ ������������ � 0.
///TODO: ��� ������ �� OneHot'�. � ���������, ����� ����� �������� � ���� '-1', � ����������� ��� 0
ULProperty<double,TPyUBitmapClassifier, ptPubParameter> ConfidenceThreshold;

/// �������� ������� � �������� ��������
UPropertyOutputData<MDMatrix<int>,TPyUBitmapClassifier, ptPubOutput> OutputClasses;

/// �������� �������. ���������� �������� �� ����� ��������, ���������� ����� � ������� �� ����� �������
/// ������ �������� - ����������� ������
UPropertyOutputData<MDMatrix<double>, TPyUBitmapClassifier> OutputConfidences;

/// �����, ����������� �� �������������, �������
ULProperty<double,TPyUBitmapClassifier, ptPubState> ClassificationTime;

protected: // ��������� ����������

UBitmap ProcessedBmp;
cv::Mat ProcessedMat;

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

/// ������������ ���� �����������
virtual bool ClassifyBitmap(UBitmap &bmp, MDVector<double> &output_confidences, double conf_thresh, int &class_id, bool &is_classified);
// --------------------------
};


}

#endif



