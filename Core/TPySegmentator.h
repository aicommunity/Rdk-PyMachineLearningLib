#ifndef RDK_TPySegmentatorH
#define RDK_TPySegmentatorH

#include "TPyComponent.h"

namespace RDK {

#define YOLOV2_INITTYPE 2
#define YOLOV3_INITTYPE 3

class TPySegmentator: public TPyComponent
{
public: // ��������
/// ������� �����������
UPropertyInputData<UBitmap, TPySegmentator> InputImage;

/// �������� ����������� c �������������� ������
UPropertyOutputData<UBitmap, TPySegmentator> OutputImage;

/// ����� �����, ������������ �������� ������, �� ������� ���������� ����
/// ubmRGB24=3 - ������� �����������
/// umbY8=400 - �����-����� �����������
ULProperty<int,TPySegmentator, ptPubParameter> ImageColorModel;

/// ������������ ������� � ������
ULProperty<std::vector<std::map<std::string, UColorT> >,TPySegmentator, ptPubParameter> ClassColors;

protected: // ��������� ����������
UGraphics Graph;

UBitmap ProcessedBmp;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPySegmentator(void);
virtual ~TPySegmentator(void);
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
virtual bool Inference(UBitmap &bmp, UBitmap &mask);
// --------------------------
};


}

#endif



