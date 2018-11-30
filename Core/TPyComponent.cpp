#define NO_IMPORT_ARRAY

#ifndef RDK_TPyComponentCPP
#define RDK_TPyComponentCPP

#include "TPyComponent.h"
#include <iostream>

namespace RDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyComponent::TPyComponent(void)
: PythonScriptFileName("PythonScriptFileName",this,&TPyComponent::SetPythonScriptFileName),
  PythonScriptPath("PythonScriptPath",this,&TPyComponent::SetPythonClassifierScriptPath),
  PythonInitialized(false)
{
}
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
bool TPyComponent::SetPythonScriptFileName(const std::string& path)
{
 Ready=false;
 return true;
}

bool TPyComponent::SetPythonClassifierScriptPath(const std::string& path)
{
 PythonScriptFileName = path;
 Ready=false;
 return true;
}

TPyComponent::~TPyComponent(void)
{
}
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
void TPyComponent::PythonInitialize(void)
{
    try
    {
        LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Python init started..."));
        PythonInitialized=false;
        py::object MainModule = py::import("__main__");  // ����������� main-scope, ��. https://docs.python.org/3/library/__main__.html
        py::object MainNamespace = MainModule.attr("__dict__");  // ��������� ������� ����

        // �������� ���� �� ����� � ����������� ������� ����
        FullPythonScriptFileName = GetEnvironment()->GetCurrentDataDir()+PythonScriptFileName->c_str();
        py::object DetectorInterfaceModule = import("detector_interface",FullPythonScriptFileName,MainNamespace);

        // ��������� ������������ ������, ����� ������� ������������ ����������� ����� � ����������� ������
        // TODO: ����� �� ����� ����� �� �������
        IntegrationInterface = DetectorInterfaceModule.attr("DetectorEmbeddingInterface");
        if(!IntegrationInterface.is_none())
         IntegrationInterfaceInstance = IntegrationInterface();

        PythonInitialized=APythonInitialize();
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail: ")+perrorStr);
        PythonInitialized=false;
    }
    catch(...)
    {
        PythonInitialized=false;
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail: Undandled exception"));
    }

    if(PythonInitialized)
     LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("...Python init finished successful!"));
    else
     LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("...Python init FAILED!"));
}

// �������������� �������� �� ��������� � ����� �������� �����
bool TPyComponent::ADefault(void)
{
 PythonInitialized=false;
 return APyDefault();
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TPyComponent::ABuild(void)
{
 if(IsInit())
  PythonInitialize();
 return APyBuild();
}

// ����� �������� ����� ��� ������ ��������
bool TPyComponent::AReset(void)
{
 if(!PythonInitialized)
  PythonInitialize();
 return APyReset();
}

// ��������� ������ ����� �������
bool TPyComponent::ACalculate(void)
{
 if(!PythonInitialized)
  return true;
 return APyCalculate();
}
// --------------------------

}
#endif



