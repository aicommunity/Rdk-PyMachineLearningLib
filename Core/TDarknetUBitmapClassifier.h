#ifndef RDK_TDarknetUBitmapClassifierH
#define RDK_TDarknetUBitmapClassifierH

#include "TPythonIntegrationInclude.h"
//#include "TPythonIntegrationUtil.h"
#include "../ThirdParty/darknet/include/darknet_utils.h"

namespace RDK {

class TDarknetUBitmapClassifier: public RDK::UNet
{
public: // ��������
/// ������� �����������
/// UPropertyInputData<UBitmap,TDarknetUBitmapClassifier> InputImage;

//������� ������� � ������� �� ������������
/// �������� ����������� (������������) ��� �������������
UPropertyInputData<std::vector<UBitmap>, TDarknetUBitmapClassifier, ptPubInput> InputImages;

/// ����� �����, ������������ �������� ������, �� ������� ���������� ����
/// ubmRGB24=3 - ������� �����������
/// umbY8=400 - �����-����� �����������
ULProperty<int,TDarknetUBitmapClassifier, ptPubParameter> ImageColorModel;

/// ���������� ������� �������� (����� ������ ����� � �������
ULProperty<int,TDarknetUBitmapClassifier, ptPubParameter> NumClasses;
/// ����� �����������, ���� �������� ����������� ��������� ��������� (probability)
ULProperty<float,TDarknetUBitmapClassifier, ptPubParameter> ProbabilityThreshold;
/// ����� �����������, ���� �������� ����������� ��������� ��������� (objectness)
ULProperty<float,TDarknetUBitmapClassifier, ptPubParameter> ObjectnessThreshold;

/// �������� ������� � �������� ��������
UPropertyOutputData<std::vector<int>,TDarknetUBitmapClassifier, ptPubOutput> OutputClasses;

/// �������� �������. ���������� �������� �� ����� ��������, ���������� ����� � ������� �� ����� �������
/// ������ �������� - ����������� ������
UPropertyOutputData<MDMatrix<double>, TDarknetUBitmapClassifier> OutputConfidences;

///���� � ����� ������������ ����
ULProperty<std::string,TDarknetUBitmapClassifier, ptPubParameter> ConfigPath;
///���� � ����� ����� ����
ULProperty<std::string,TDarknetUBitmapClassifier, ptPubParameter> WeightsPath;

protected: // ���������� ���������

UGraphics Graph;
UBitmap Canvas;

bool Initialized;

network *Network;
//layer* TopLayer;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TDarknetUBitmapClassifier(void);
virtual ~TDarknetUBitmapClassifier(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TDarknetUBitmapClassifier* New(void);
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
bool Initialize(void);

virtual void AInit(void);
virtual void AUnInit(void);

// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ADefault(void);

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool ABuild(void);

// ����� �������� ����� ��� ������ ��������
virtual bool AReset(void);

// ��������� ������ ����� �������
virtual bool ACalculate(void);
// --------------------------
image UBitmapToImage(const UBitmap& ub);
};


}

#endif



