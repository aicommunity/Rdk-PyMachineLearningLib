#ifndef TPyObjectDetectorBasic
#define RDK_TPyObjectDetectorBasicH

#include "TPythonIntegrationInclude.h"
#include "TPythonIntegrationUtil.h"

namespace RDK {

class TPyObjectDetectorBasic: public RDK::UNet
{
public: // ��������
/// ������� �����������
UPropertyInputData<UBitmap, TPyObjectDetectorBasic, ptPubInput> InputImage;
UPropertyInputData<UBitmap, TPyObjectDetectorBasic, ptPubOutput> OutputImage;

/// ����� �����, ������������ �������� ������, �� ������� ���������� ����
/// ubmRGB24=3 - ������� �����������
/// umbY8=400 - �����-����� �����������
ULProperty<int,TPyObjectDetectorBasic, ptPubParameter> ImageColorModel;

/// ���������� ������� �������� (����� ������ ����� � �������
//ULProperty<int,TPyObjectDetectorBasic, ptPubParameter> NumClasses;model_path

///��� �������������: 1 - ����������, �����������, ����� ���� � ������, ������, ������� �������, �������� � ��������
///                   2 - ������������� ����� ���� � ������������� � ���� �����
//ULProperty<int,TPyObjectDetectorBasic, ptPubParameter> InitializationTypeYOLO;

///���� � ����� ������������ ����
//ULProperty<std::string,TPyObjectDetectorBasic, ptPubParameter> ConfigPathYOLO;
///���� � ����� ����� ����
//ULProperty<std::string,TPyObjectDetectorBasic, ptPubParameter> WeightsPathYOLO;



///���� � ������ ����
ULProperty<std::string,TPyObjectDetectorBasic, ptPubParameter> ModelPathYOLO;
///���� � ����� � ������� (�������� ������ ���� ���������� ��������)
ULProperty<std::string,TPyObjectDetectorBasic, ptPubParameter> AnchorsPathYOLO;
///���� � ����� � �������� ������� ��� ������������ ����
ULProperty<std::string,TPyObjectDetectorBasic, ptPubParameter> ClassesPathYOLO;
///������ ������� �������
ULProperty<std::vector<std::string>,TPyObjectDetectorBasic, ptPubParameter> TargetClassesYOLO;
///��������� ������ ������� �� ���������������� �����
ULProperty<bool,TPyObjectDetectorBasic, ptPubParameter> LoadTargetClassesYOLO;
///������ ����� ������� (�������� ������ ������� � ����� ClassesPathYOLO �� ���� �����)
///��������!!! ����� ������� ������ ��������������� ����� ������� � ClassesPathYOLO
ULProperty<std::vector<std::string>,TPyObjectDetectorBasic, ptPubParameter> ChangeClassesYOLO;

///������ ������� �� ���������
///ULProperty<std::map<int, int>,TPyObjectDetectorBasic, ptPubParameter> ClassIndicesExchange;


/// �������� ������� � �������� ��������
//UPropertyOutputData<std::vector<int>,TPyObjectDetectorBasic, ptPubOutput> OutputClasses;

/// �������� �������. ���������� �������� �� ����� ��������, ���������� ����� � ������� �� ����� �������
/// ������ �������:
/// ������ �� ���������� ��������
/// ������ 4+1=Left; Top; Right; Bottom; ClassNumber
UPropertyOutputData<MDMatrix<double>, TPyObjectDetectorBasic> OutputObjects;

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
TPyObjectDetectorBasic(void);
virtual ~TPyObjectDetectorBasic(void);
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
virtual TPyObjectDetectorBasic* New(void);
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
};


}

#endif



