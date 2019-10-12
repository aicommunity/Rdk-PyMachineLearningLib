#ifndef RDK_TPyAggregateClassifierH
#define RDK_TPyAggregateClassifierH

#include "TPythonIntegrationInclude.h"
#include "TPythonIntegrationUtil.h"

namespace RDK {

class TPyAggregateClassifier: public RDK::UNet
{
public: // ��������
/// ������� �����������
UPropertyInputData<UBitmap,TPyAggregateClassifier> InputImage;

/// ������� ����
ULProperty<std::string,TPyAggregateClassifier, ptPubParameter> PythonScriptFileName;

/// ������� ������� � ������� �� ������������
/// ������ ������� �������� [Left Top Right Bottom] ������� � ������� ����������� � AggrIdMatrix
UPropertyInputData<MDMatrix<int>, TPyAggregateClassifier, ptPubInput> AggrRectsMatrix;

/// ������ ������� �������� [AggrID] ������� � ������� ����������� � AggrRectsMatrix
UPropertyInputData<MDMatrix<int>, TPyAggregateClassifier, ptPubInput> AggrIdMatrix;

/// �������� ������� � ������� ��������
UPropertyOutputData<MDMatrix<int>,TPyAggregateClassifier> Detections;

UPropertyOutputData<UBitmap,TPyAggregateClassifier> DebugImage;

boost::python::object IntegrationInterface;
boost::python::object IntegrationInterfaceInstance;

protected: // ���������� ���������

UGraphics Graph;
UBitmap Canvas;
bool Initialized;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyAggregateClassifier(void);
virtual ~TPyAggregateClassifier(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TPyAggregateClassifier* New(void);
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



