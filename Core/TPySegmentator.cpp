#define NO_IMPORT_ARRAY

#ifndef RDK_TPySegmentatorCPP
#define RDK_TPySegmentatorCPP

#include "TPySegmentator.h"
#include <iostream>

namespace RDK {


// ������
// --------------------------
// ������������ � �����������
// --------------------------
TPySegmentator::TPySegmentator(void):
  ImageColorModel("ImageColorModel",this)
{

}


TPySegmentator::~TPySegmentator(void)
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
bool TPySegmentator::APyDefault(void)
{
 PythonModuleName="segmentator_interface";
 PythonClassName="SegmentatorEmbeddingInterface";

 return APyDefault2();
}

bool TPySegmentator::APyDefault2(void)
{
 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TPySegmentator::APyBuild(void)
{
 return APyBuild2();
}

bool TPySegmentator::APyBuild2(void)
{
 return true;
}

// ����� �������� ����� ��� ������ ��������
bool TPySegmentator::APyReset(void)
{
 return APyReset2();
}

bool TPySegmentator::APyReset2(void)
{
 return true;
}


bool TPySegmentator::Inference(UBitmap &bmp, UBitmap &mask)
{
 return true;
}

// --------------------------

}
#endif



