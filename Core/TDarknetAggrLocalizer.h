#ifndef RDK_TDarknetAggrLocalizerH
#define RDK_TDarknetAggrLocalizerH

#include "TPythonIntegrationInclude.h"
//#include "TPythonIntegrationUtil.h"
#include "../ThirdParty/darknet/include/darknet_utils.h"

namespace RDK {

class TDarknetAggrLocalizer: public RDK::UNet
{
public: // ��������
/// ������� �����������
/// UPropertyInputData<UBitmap,TDarknetAggrLocalizer> InputImage;

//������� ������� � ������� �� ������������
/// �������� ����������� (������������) ��� �������������
UPropertyInputData<std::vector<UBitmap>, TDarknetAggrLocalizer, ptPubInput> InputImages;

/// ����� �����, ������������ �������� ������, �� ������� ���������� ����
/// ubmRGB24=3 - ������� �����������
/// umbY8=400 - �����-����� �����������
ULProperty<int,TDarknetAggrLocalizer, ptPubParameter> ImageColorModel;

/// ���������� ������� �������� (����� ������ ����� � �������
ULProperty<int,TDarknetAggrLocalizer, ptPubParameter> NumClasses;
/// ����� �����������, ���� �������� ����������� ��������� ��������� (probability)
ULProperty<float,TDarknetAggrLocalizer, ptPubParameter> ProbabilityThreshold;
/// ����� �����������, ���� �������� ����������� ��������� ��������� (objectness)
ULProperty<float,TDarknetAggrLocalizer, ptPubParameter> ObjectnessThreshold;

/// �������� ������� � �������� ��������
UPropertyOutputData<std::vector<int>,TDarknetAggrLocalizer, ptPubOutput> OutputClasses;

/// �������� �������. ���������� �������� �� ����� ��������, ���������� ����� � ������� �� ����� �������
/// ������ �������� - ����������� ������
UPropertyOutputData<MDMatrix<double>, TDarknetAggrLocalizer> OutputConfidences;

///���� � ����� ������������ ����
ULProperty<std::string,TDarknetAggrLocalizer, ptPubParameter> ConfigPath;
///���� � ����� ����� ����
ULProperty<std::string,TDarknetAggrLocalizer, ptPubParameter> WeightsPath;

protected: // ���������� ���������

UGraphics Graph;
UBitmap Canvas;

bool Initialized;

network *Network;
layer* TopLayer;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TDarknetAggrLocalizer(void);
virtual ~TDarknetAggrLocalizer(void);
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TDarknetAggrLocalizer* New(void);
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



