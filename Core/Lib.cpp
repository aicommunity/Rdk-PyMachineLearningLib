#ifndef RDK_UPyMachineLearningLib_CPP
#define RDK_UPyMachineLearningLib_CPP

#include "Lib.h"

namespace RDK {

UPyMachineLearningLib PyMachineLearningLib;



// --------------------------
// Конструкторы и деструкторы
// --------------------------
UPyMachineLearningLib::UPyMachineLearningLib(void)
 : RDK::ULibrary("PyMachineLearningLib","1.0")
{
}
// --------------------------

// --------------------------
// Методы заполенения бибилиотеки
// --------------------------
// Заполняет массив ClassSamples готовыми экземплярами образцов и их именами.
// Не требуется предварительная очистка массива и уборка памяти.
void UPyMachineLearningLib::CreateClassSamples(RDK::UStorage *storage)
{
 UploadClass<TPythonIntegration>("TPythonIntegration", "PythonIntegration");
}
// --------------------------

}

#endif


