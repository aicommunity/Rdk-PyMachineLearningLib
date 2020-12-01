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
//  PythonScriptPath("PythonScriptPath",this,&TPyComponent::SetPythonClassifierScriptPath),
  PythonModuleName("PythonModuleName",this, &TPyComponent::SetPythonModuleName),
  PythonClassName("PythonClassName",this, &TPyComponent::SetPythonClassName),
  UseFullPath("UseFullPath", this),
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
/*
bool TPyComponent::SetPythonClassifierScriptPath(const std::string& path)
{
 PythonScriptFileName = path;
 Ready=false;
 return true;
}
*/
bool TPyComponent::SetPythonModuleName(const std::string& path)
{
 Ready=false;
 return true;
}

bool TPyComponent::SetPythonClassName(const std::string& path)
{
 Ready=false;
 return true;
}


TPyComponent::~TPyComponent(void)
{
    //Py_BLOCK_GIL
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

        if(PythonModuleName->empty())
        {
         LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("PythonModuleName is empty!"));
         return;
        }

        if(PythonClassName->empty())
        {
         LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("PythonClassName is empty!"));
         return;
        }

        if(!PyEval_ThreadsInitialized())
        {
         LogMessageEx(RDK_EX_FATAL,__FUNCTION__,std::string("Python Py_Initialize didn't called!"));
         LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("Python init fail"));
         return;
        }
        Py_BLOCK_GIL

        py::object MainModule = py::import("__main__");  // ����������� main-scope, ��. https://docs.python.org/3/library/__main__.html
        py::object MainNamespace = MainModule.attr("__dict__");  // ��������� ������� ����

        // �������� ���� �� ����� � ����������� ������� ����
        if(*UseFullPath)
        {
         FullPythonScriptFileName = PythonScriptFileName->c_str();
        }
        else
        {
         FullPythonScriptFileName = GetEnvironment()->GetCurrentDataDir()+PythonScriptFileName->c_str();
        }
        py::object DetectorInterfaceModule = import(*PythonModuleName,FullPythonScriptFileName,MainNamespace);

        // ��������� ������������ ������, ����� ������� ������������ ����������� ����� � ����������� ������
        // TODO: ����� �� ����� ����� �� �������
        IntegrationInterface = DetectorInterfaceModule.attr(PythonClassName->c_str());
        if(!IntegrationInterface.is_none())
         IntegrationInterfaceInstance = IntegrationInterface(); // DetectorEmbeddingInterface

        PythonInitialized=APythonInitialize();
    }
    catch (py::error_already_set const &)
    {
        Py_UNBLOCK_GIL
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("Python init fail: ")+perrorStr);
        PythonInitialized=false;
    }
    catch(...)
    {
        Py_UNBLOCK_GIL
        PythonInitialized=false;
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("Python init fail: Unhandled exception"));
    }

    if(PythonInitialized)
     LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("...Python init finished successful!"));
    else
     LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("...Python init FAILED!"));

    Py_UNBLOCK_GIL
}

// �������������� �������� �� ��������� � ����� �������� �����
bool TPyComponent::ADefault(void)
{
 PythonInitialized=false;
 UseFullPath=true;
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
bool TPyComponent::ABeforeCalculate(void)
{
 if(!PythonInitialized)
    return true;

 return true;
}
// --------------------------

}
#endif



