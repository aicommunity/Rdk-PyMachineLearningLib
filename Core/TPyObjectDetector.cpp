#define NO_IMPORT_ARRAY

#ifndef RDK_TPyObjectDetectorCPP
#define RDK_TPyObjectDetectorCPP

#include "TPyObjectDetector.h"
#include <iostream>

namespace RDK {


// ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyObjectDetector::TPyObjectDetector(void)
:ImageColorModel("ImageColorModel",this)
{

}


TPyObjectDetector::~TPyObjectDetector(void)
{
}
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������

// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool TPyObjectDetector::APyDefault(void)
{
 PythonModuleName="detector_interface";
 PythonClassName="DetectorEmbeddingInterface";

 return APyDefault2();
}

bool TPyObjectDetector::APyDefault2(void)
{
 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TPyObjectDetector::APyBuild(void)
{
 return APyBuild2();
}

bool TPyObjectDetector::APyBuild2(void)
{
 return true;
}

// ����� �������� ����� ��� ������ ��������
bool TPyObjectDetector::APyReset(void)
{
 return APyReset2();
}

bool TPyObjectDetector::APyReset2(void)
{
 return true;
}


bool TPyObjectDetector::Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities)
{
 return true;
}

// --------------------------

}
#endif



