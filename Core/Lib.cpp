#ifndef RDK_UPyMachineLearningLib_CPP
#define RDK_UPyMachineLearningLib_CPP

#include "Lib.h"

namespace RDK {

UPyMachineLearningLib PyMachineLearningLib;



// --------------------------
// ������������ � �����������
// --------------------------
UPyMachineLearningLib::UPyMachineLearningLib(void)
 : ULibrary("PyMachineLearningLib","1.0")
{
}
// --------------------------

// --------------------------
// ������ ����������� �����������
// --------------------------
// ��������� ������ ClassSamples �������� ������������ �������� � �� �������.
// �� ��������� ��������������� ������� ������� � ������ ������.
void UPyMachineLearningLib::CreateClassSamples(UStorage *storage)
{
 //UploadClass<TPythonIntegration>("TPythonIntegration", "PythonIntegration");

    /*UContainer *cont=0;

 cont=new TPythonIntegration;
 cont->Default();
 cont->SetName("PythonIntegration");
 UploadClass("TPythonIntegration",cont);*/

 UploadClass<TPyUBitmapClassifier>("TPyUBitmapClassifier","PyUBitmapClassifier");
 UploadClass<TPyAggregateClassifier>("TPyAggregateClassifier","PyAggregateClassifier");

}
// --------------------------

}

#endif


