#ifndef RDK_UPyMachineLearningLib_H
#define RDK_UPyMachineLearningLib_H

#include "../../../Rdk/Deploy/Include/rdk.h"
//#include "TPythonIntegration.h"
#include "TPyAggregateClassifier.h"

namespace RDK {

class RDK_LIB_TYPE UPyMachineLearningLib: public RDK::ULibrary
{
public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UPyMachineLearningLib(void);
// --------------------------

// --------------------------
// Методы заполенения бибилиотеки
// --------------------------
// Заполняет массив ClassSamples готовыми экземплярами образцов и их именами.
// Не требуется предварительная очистка массива и уборка памяти.
virtual void CreateClassSamples(RDK::UStorage *storage);
// --------------------------
};

extern RDK_LIB_TYPE UPyMachineLearningLib PyMachineLearningLib;

}

#endif


