#ifndef RDK_UPyMachineLearningLib_H
#define RDK_UPyMachineLearningLib_H

#include "../../../Rdk/Deploy/Include/rdk.h"

#ifdef RDK_USE_PYTHON
#include "TPyUBitmapClassifier.h"
#include "TPyAggregateClassifier.h"
#include "TPyObjectDetectorYoloEx.h"
#include "TPyObjectDetectorYolo.h"
#include "TPyObjectDetectorSqueezeDet.h"
#include "TPySegmentatorProtobuf.h"
#include "TPySegmentatorUNet.h"
#include "TPyClassifierTrainer.h"
#endif

namespace RDK {

class RDK_LIB_TYPE UPyMachineLearningLib: public RDK::ULibrary
{
private:
/// Флаг, сигнализирующий что питон-подсистема была инициализирована
static bool PythonInitFlag;
public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UPyMachineLearningLib(void);
// --------------------------

// --------------------------
// Методы заполенения бибилиотеки
// --------------------------
/// Однократная инициализация python-подистемы
static void PythonInit(void);

// Заполняет массив ClassSamples готовыми экземплярами образцов и их именами.
// Не требуется предварительная очистка массива и уборка памяти.
virtual void CreateClassSamples(RDK::UStorage *storage);
// --------------------------
};

extern RDK_LIB_TYPE UPyMachineLearningLib PyMachineLearningLib;

}

#endif


