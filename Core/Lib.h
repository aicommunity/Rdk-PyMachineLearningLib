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
#include "TPyDetectorTrainer.h"
#include "TPySegmenterTrainer.h"
#include "TPyPredictSort.h"
#include "TPyDetPredict.h"
#endif

namespace RDK {

class RDK_LIB_TYPE UPyMachineLearningLib: public RDK::ULibrary
{
private:
/// ����, ��������������� ��� �����-���������� ���� ����������������
static bool PythonInitFlag;
public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
UPyMachineLearningLib(void);
~UPyMachineLearningLib(void);
// --------------------------
PyGILState_STATE mGilState;     // ���������� ��������� GIL
PyThreadState* mThreadState;    // ���������� ��������� ��������� ������
// --------------------------
// ������ ����������� �����������
// --------------------------
/// ����������� ������������� python-���������
static void PythonInit(void);

// ��������� ������ ClassSamples �������� ������������ �������� � �� �������.
// �� ��������� ��������������� ������� ������� � ������ ������.
virtual void CreateClassSamples(RDK::UStorage *storage);
// --------------------------
};

extern RDK_LIB_TYPE UPyMachineLearningLib PyMachineLearningLib;

}

#endif


