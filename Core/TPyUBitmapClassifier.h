#ifndef PY_ARRAY_UNIQUE_SYMBOL
#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API
#endif
#ifndef RDK_TPyUBitmapClassifierH
#define RDK_TPyUBitmapClassifierH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "TPythonIntegrationInclude.h"


namespace RDK {

class TPyUBitmapClassifier: public RDK::UNet
{
public: // ��������
/// ������� �����������
/// UPropertyInputData<UBitmap,TPyUBitmapClassifier> InputImage;

/// ������� �����������
//ULProperty<std::string,TPyUBitmapClassifier, ptPubParameter> PythonScriptFileName;

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

boost::python::object IntegrationInterface;
boost::python::object IntegrationInterfaceInstance;

protected: // ���������� ���������

UGraphics Graph;
UBitmap Canvas;
bool Initialized;
std::string PythonScriptFileName;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyUBitmapClassifier(void);
virtual ~TPyUBitmapClassifier(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------

bool SetPythonClassifierScriptPath(const std::string& path);
const std::string &GetPythonClassifierScriptPath(void) const;

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

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
};


}

#endif



