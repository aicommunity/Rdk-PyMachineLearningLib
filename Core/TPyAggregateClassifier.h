#ifndef RDK_TPyAggregateClassifierH
#define RDK_TPyAggregateClassifierH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>

namespace RDK {

class TPyAggregateClassifier: public RDK::UNet
{
public: // ��������
/// ��������
ULProperty<int,TPyAggregateClassifier> Param1;

/// ������� �����������
UPropertyInputData<UBitmap,TPyAggregateClassifier> InputImage;

/// ������� �����������
ULProperty<std::string,TPyAggregateClassifier, ptPubParameter> InputFile;
/// �������� ������� � ������� ��������
UPropertyOutputData<MDMatrix<int>,TPyAggregateClassifier> Detections;

/// �������� ������� � ������� ��������
UPropertyOutputData<MDMatrix<int>,TPyAggregateClassifier> DetectionClass;

/// �������� ������� � ������� ��������
UPropertyOutputData<MDMatrix<double>,TPyAggregateClassifier> DetectionReliability;

UPropertyOutputData<UBitmap,TPyAggregateClassifier> DebugImage;

boost::python::object IntegrationInterface;
boost::python::object IntegrationInterfaceInstance;

protected: // ���������� ���������

UGraphics Graph;

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



