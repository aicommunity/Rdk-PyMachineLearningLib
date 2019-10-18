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
TPySegmentator::TPySegmentator(void)
: InputImage("InputImage",this),
  ImageColorModel("ImageColorModel",this),
  ClassColors("ClassColors", this),
  OutputImage("DebugImage",this)


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

// ��������� ������ ����� �������
bool TPySegmentator::APyCalculate(void)
{
 if(!InputImage.IsConnected())
  return true;

 if(ImageColorModel!=InputImage->GetColorModel())
 {
     LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("Incorrect input image color model. Need "+sntoa(*ImageColorModel)+" got: ")+sntoa(InputImage->GetColorModel()));
     return true;
 }

 ProcessedBmp=*InputImage;

 if(ProcessedBmp.GetData()==NULL)
  return true;

 UBitmap &mask = (*OutputImage);

 try
 {
  if(!Inference(ProcessedBmp, mask))
  {
   ; // TODO: ��� ��� �� ������ ����
  }
 }
 catch (py::error_already_set const &)
 {
  std::string perrorStr = parse_python_exception();
  LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPySegmentator error: ")+perrorStr);
 }

 return true;
}

bool TPySegmentator::Inference(UBitmap &bmp, UBitmap &mask)
{
 return true;
}

// --------------------------

}
#endif



