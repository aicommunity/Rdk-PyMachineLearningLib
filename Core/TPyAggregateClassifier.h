#ifndef RDK_TPyAggregateClassifierH
#define RDK_TPyAggregateClassifierH

#include "../../../Rdk/Deploy/Include/rdk.h"
#ifndef BOOST_PYTHON_STATIC_LIB
#define BOOST_PYTHON_STATIC_LIB
#endif
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>

namespace RDK {

class TPyAggregateClassifier: public RDK::UNet
{
public: // ��������
/// ������� �����������
UPropertyInputData<UBitmap,TPyAggregateClassifier> InputImage;

/// ������� �����������
ULProperty<std::string,TPyAggregateClassifier, ptPubParameter> InputFile;

//������� ������� � ������� �� ������������
/// ������ ������� �������� [Left Top Right Bottom] ������� � ������� ����������� � AggrIdMatrix
UPropertyInputData<MDMatrix<int>, TPyAggregateClassifier, ptPubInput> AggrRectsMatrix;
/// ������ ������� �������� [AggrID] ������� � ������� ����������� � AggrRectsMatrix
UPropertyInputData<MDMatrix<int>, TPyAggregateClassifier, ptPubInput> AggrIdMatrix;

/// �������� ������� � ������� ��������
UPropertyOutputData<MDMatrix<int>,TPyAggregateClassifier> Detections;


/// �������� ������� � ������� ��������
//UPropertyOutputData<MDMatrix<double>,TPyAggregateClassifier> DetectionReliability;

UPropertyOutputData<UBitmap,TPyAggregateClassifier> DebugImage;

boost::python::object IntegrationInterface;
boost::python::object IntegrationInterfaceInstance;

protected: // ���������� ���������

UGraphics Graph;
UBitmap Canvas;

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



