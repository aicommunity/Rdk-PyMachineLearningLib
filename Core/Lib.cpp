#ifndef RDK_UPyMachineLearningLib_CPP
#define RDK_UPyMachineLearningLib_CPP

#include "Lib.h"

namespace RDK {

UPyMachineLearningLib PyMachineLearningLib;



// --------------------------
// Конструкторы и деструкторы
// --------------------------
UPyMachineLearningLib::UPyMachineLearningLib(void)
 : ULibrary("PyMachineLearningLib","1.0")
{
}
// --------------------------

// --------------------------
// Методы заполенения бибилиотеки
// --------------------------
// Заполняет массив ClassSamples готовыми экземплярами образцов и их именами.
// Не требуется предварительная очистка массива и уборка памяти.
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


