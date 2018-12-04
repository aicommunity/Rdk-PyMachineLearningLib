#ifndef RDK_TDarknetObjectDetectorH
#define RDK_TDarknetObjectDetectorH

#include "TPythonIntegrationInclude.h"
#include "TPythonIntegrationUtil.h"
#include "../ThirdParty/darknet/include/darknet_utils.h"

namespace RDK {

#define YOLOV2_INITTYPE 2
#define YOLOV3_INITTYPE 3

class TDarknetObjectDetector: public RDK::UNet
{
public: // ��������
/// ������� �����������
UPropertyInputData<UBitmap, TDarknetObjectDetector, ptPubInput> InputImage;
UPropertyInputData<UBitmap, TDarknetObjectDetector, ptPubOutput> OutputImage;

/// ����� �����, ������������ �������� ������, �� ������� ���������� ����
/// ubmRGB24=3 - ������� �����������
/// umbY8=400 - �����-����� �����������
ULProperty<int,TDarknetObjectDetector, ptPubParameter> ImageColorModel;

/// ���������� ������� �������� (����� ������ ����� � �������)
ULProperty<int,TDarknetObjectDetector, ptPubParameter> NumClasses;
/// ����� �����������, ���� �������� ����������� ��������� ��������� (probability)
ULProperty<float,TDarknetObjectDetector, ptPubParameter> ProbabilityThreshold;
/// ����� �����������, ���� �������� ����������� ��������� ��������� (objectness)
ULProperty<float,TDarknetObjectDetector, ptPubParameter> ObjectnessThreshold;
///�������� ���������� �� ������� �������
ULProperty<bool,TDarknetObjectDetector, ptPubParameter> FilterClassesList;
///������
ULProperty<std::vector<int>,TDarknetObjectDetector, ptPubParameter> ClassesList;

///��� �������������: 2 - YOLOV2
///                   3 - YOLOV3
//ULProperty<int,TDarknetObjectDetector, ptPubParameter> InitializationTypeYOLO;

///���� � ����� ������������ ����
ULProperty<std::string,TDarknetObjectDetector, ptPubParameter> ConfigPath;
///���� � ����� ����� ����
ULProperty<std::string,TDarknetObjectDetector, ptPubParameter> WeightsPath;



///���� � ������ ����
//ULProperty<std::string,TDarknetObjectDetector, ptPubParameter> ModelPathYOLO;
///���� � ����� � ������� (�������� ������ ���� ���������� ��������)
//ULProperty<std::string,TDarknetObjectDetector, ptPubParameter> AnchorsPathYOLO;
///���� � ����� � �������� ������� ��� ������������ ����
//ULProperty<std::string,TDarknetObjectDetector, ptPubParameter> ClassesPathYOLO;

///��������� ������ ������� �� ���������������� �����
//ULProperty<bool,TDarknetObjectDetector, ptPubParameter> LoadTargetClassesYOLO;
///������ ����� ������� (�������� ������ ������� � ����� ClassesPathYOLO �� ���� �����)
///��������!!! ����� ������� ������ ��������������� ����� ������� � ClassesPathYOLO
//ULProperty<std::vector<std::string>,TDarknetObjectDetector, ptPubParameter> ChangeClassesYOLO;

///������ ������� �� ���������
///ULProperty<std::map<int, int>,TDarknetObjectDetector, ptPubParameter> ClassIndicesExchange;


/// �������� ������� � �������� ��������
//UPropertyOutputData<std::vector<int>,TDarknetObjectDetector, ptPubOutput> OutputClasses;

/// �������� �������. ���������� �������� �� ����� ��������, ���������� ����� � ������� �� ����� �������
/// ������ �������:
/// ������ �� ���������� ��������
/// ������ 4+2=Left; Top; Right; Bottom; Confidense, ClassNumber
UPropertyOutputData<MDMatrix<double>, TDarknetObjectDetector> OutputObjects;

protected: // ���������� ���������

UGraphics Graph;
UBitmap Canvas;
bool Initialized;
std::string PythonScriptFileName;

int NumTargetClassesYOLO;
int NumChangeClassesYOLO;

std::vector<std::string> ClassedList;

network *Network;
layer TopLayer;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TDarknetObjectDetector(void);
virtual ~TDarknetObjectDetector(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------

//bool SetPythonClassifierScriptPath(const std::string& path);
//const std::string &GetPythonClassifierScriptPath(void) const;

/*
bool SetNumTargetClassesYOLO(const int& num);
const int& GetNumTargetClassesYOLO(void) const;

bool SetNumChangeClassesYOLO(const int& num);
const int& GetNumChangeClassesYOLO(void) const;
*/
// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TDarknetObjectDetector* New(void);
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



