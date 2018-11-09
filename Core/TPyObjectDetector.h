#ifndef RDK_TPyObjectDetectorH
#define RDK_TPyObjectDetectorH

#include "TPythonIntegrationInclude.h"
#include "TPythonIntegrationUtil.h"

namespace RDK {

class TPyObjectDetector: public RDK::UNet
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

///��� �������������: 1 - ����������, �����������, ����� ���� � ������, ������, ������� �������, �������� � ��������
///                   2 - ������������� ����� ���� � ������������� � ���� �����
ULProperty<int,TPyObjectDetector, ptPubParameter> InitializationTypeYOLO;

///���� � ����� ������������ ����
ULProperty<std::string,TPyObjectDetector, ptPubParameter> ConfigPathYOLO;
///���� � ����� ����� ����
ULProperty<std::string,TPyObjectDetector, ptPubParameter> WeightsPathYOLO;



///���� � ������ ����
ULProperty<std::string,TPyObjectDetector, ptPubParameter> ModelPathYOLO;
///���� � ����� � ������� (�������� ������ ���� ���������� ��������)
ULProperty<std::string,TPyObjectDetector, ptPubParameter> AnchorsPathYOLO;
///���� � ����� � �������� ������� ��� ������������ ����
ULProperty<std::string,TPyObjectDetector, ptPubParameter> ClassesPathYOLO;
///������ ������� �������
ULProperty<std::vector<std::string>,TPyObjectDetector, ptPubParameter> TargetClassesYOLO;
///��������� ������ ������� �� ���������������� �����
ULProperty<bool,TPyObjectDetector, ptPubParameter> LoadTargetClassesYOLO;
///������ ����� ������� (�������� ������ ������� � ����� ClassesPathYOLO �� ���� �����)
///��������!!! ����� ������� ������ ��������������� ����� ������� � ClassesPathYOLO
ULProperty<std::vector<std::string>,TPyObjectDetector, ptPubParameter> ChangeClassesYOLO;

///������ ������� �� ���������
///ULProperty<std::map<int, int>,TPyObjectDetector, ptPubParameter> ClassIndicesExchange;


/// �������� ������� � �������� ��������
//UPropertyOutputData<std::vector<int>,TPyObjectDetector, ptPubOutput> OutputClasses;

/// �������� �������. ���������� �������� �� ����� ��������, ���������� ����� � ������� �� ����� �������
/// ������ �������:
/// ������ �� ���������� ��������
/// ������ 4+1=Left; Top; Right; Bottom; ClassNumber
UPropertyOutputData<MDMatrix<double>, TPyObjectDetector> OutputObjects;

boost::python::object IntegrationInterface;
boost::python::object IntegrationInterfaceInstance;

protected: // ���������� ���������

UGraphics Graph;
UBitmap Canvas;
bool Initialized;
std::string PythonScriptFileName;

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

// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------

bool SetPythonClassifierScriptPath(const std::string& path);
const std::string &GetPythonClassifierScriptPath(void) const;

bool SetNumTargetClassesYOLO(const int& num);
const int& GetNumTargetClassesYOLO(void) const;

bool SetNumChangeClassesYOLO(const int& num);
const int& GetNumChangeClassesYOLO(void) const;

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

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
bool Initialize(void);
//virtual void AInit(void);
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



