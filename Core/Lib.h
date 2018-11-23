#ifndef RDK_UPyMachineLearningLib_H
#define RDK_UPyMachineLearningLib_H

#include "../../../Rdk/Deploy/Include/rdk.h"
//#include "TPythonIntegration.h"
#include "TPyUBitmapClassifier.h"
#include "TPyAggregateClassifier.h"
#include "TPyObjectDetectorBasic.h"
#include "TPyObjectDetector.h"
#ifdef __GNUC__
#include "TDarknetObjectDetector.h"
#endif

namespace RDK {

class RDK_LIB_TYPE UPyMachineLearningLib: public RDK::ULibrary
{
public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
UPyMachineLearningLib(void);
// --------------------------

// --------------------------
// ������ ����������� �����������
// --------------------------
// ��������� ������ ClassSamples �������� ������������ �������� � �� �������.
// �� ��������� ��������������� ������� ������� � ������ ������.
virtual void CreateClassSamples(RDK::UStorage *storage);
// --------------------------
};

extern RDK_LIB_TYPE UPyMachineLearningLib PyMachineLearningLib;

}

#endif


