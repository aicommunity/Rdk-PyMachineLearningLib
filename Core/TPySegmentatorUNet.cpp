#define NO_IMPORT_ARRAY

#ifndef RDK_TPySegmentatorUNetCPP
#define RDK_TPySegmentatorUNetCPP

#include "TPySegmentatorUNet.h"
#include <iostream>

namespace RDK {


// ������
// --------------------------
// ������������ � �����������
// --------------------------
TPySegmentatorUNet::TPySegmentatorUNet(void):
  WeightsPath("WeightsPath",this),
  RespondThreshold("RespondThreshold", this)
{

}


TPySegmentatorUNet::~TPySegmentatorUNet(void)
{
}
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
TPySegmentatorUNet* TPySegmentatorUNet::New(void)
{
 return new TPySegmentatorUNet;
}
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
bool TPySegmentatorUNet::APythonInitialize(void)
{
    try
    {
        py::object initialize;
        initialize = IntegrationInterfaceInstance.attr("initialize_config")(GetEnvironment()->GetCurrentDataDir()+*WeightsPath);


        if(!initialize.is_none())
        {
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init success"));
            return true;
        }
        else
        {
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python initialization failed"));
            return false;
        }
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail: ")+perrorStr);
        return false;
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail: Undandled exception"));
        return false;
    }

    LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("...Python init finished successful!"));
    return true;
}

// �������������� �������� �� ��������� � ����� �������� �����
bool TPySegmentatorUNet::APyDefault2(void)
{
 (*RespondThreshold)=64.0;
 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TPySegmentatorUNet::APyBuild2(void)
{
 return true;
}

// ����� �������� ����� ��� ������ ��������
bool TPySegmentatorUNet::APyReset2(void)
{
 return true;
}

// ��������� ������ ����� �������
// ��������� �����������
bool TPySegmentatorUNet::Inference(UBitmap &bmp, UBitmap &mask)
{
 try
 {
  py::object retval = IntegrationInterfaceInstance.attr("inference")(bmp);

  cv::Mat res_grayscale = pbcvt::fromNDArrayToMat(retval.ptr());

  cv::Mat result_grey;
  cv::threshold(res_grayscale, result_grey, (*RespondThreshold), 255, cv::ThresholdTypes::THRESH_BINARY);


  int type = result_grey.type();
  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);
  if(depth==CV_8U && chans==3)
  {
      UBitmap temp;
      temp.SetRes(result_grey.cols, result_grey.rows, RDK::ubmRGB24);
      temp.AttachBuffer(result_grey.data);
      mask.SetRes(result_grey.cols, result_grey.rows, RDK::ubmRGB24);
      temp.ConvertTo(mask);
      temp.DetachBuffer();
      mask.SwapRGBChannels();
  }
  else if(depth==CV_8U && chans==1)
  {
      UBitmap temp;
      temp.SetRes(result_grey.cols, result_grey.rows, RDK::ubmY8);
      temp.AttachBuffer(result_grey.data);
      mask.SetRes(result_grey.cols, result_grey.rows, RDK::ubmY8);
      temp.ConvertTo(mask);
      temp.DetachBuffer();
  }
  else
  {
    LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPySegmentatorUNet: Received numpy array with incorrect color model"));
  }

 }
 catch (py::error_already_set const &)
 {
  std::string perrorStr = parse_python_exception();
  LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPySegmentatorUNet error: ")+perrorStr);
 }

 return true;
}
// --------------------------

}
#endif



