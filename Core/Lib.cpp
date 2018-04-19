#ifndef RDK_UPyMachineLearningLib_CPP
#define RDK_UPyMachineLearningLib_CPP

#include "Lib.h"

namespace RDK {

UPyMachineLearningLib PyMachineLearningLib;



// --------------------------
// ������������ � �����������
// --------------------------
UPyMachineLearningLib::UPyMachineLearningLib(void)
 : RDK::ULibrary("PyMachineLearningLib","1.0")
{
}
// --------------------------

// --------------------------
// ������ ����������� �����������
// --------------------------
// ��������� ������ ClassSamples �������� ������������ �������� � �� �������.
// �� ��������� ��������������� ������� ������� � ������ ������.
void UPyMachineLearningLib::CreateClassSamples(RDK::UStorage *storage)
{
 UploadClass<TPythonIntegration>("TPythonIntegration", "PythonIntegration");
}
// --------------------------

}

#endif


