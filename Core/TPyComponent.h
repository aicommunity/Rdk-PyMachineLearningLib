#ifndef RDK_TPyComponentH
#define RDK_TPyComponentH

#include "TPythonIntegrationInclude.h"
#include "TPythonIntegrationUtil.h"

// ��������� ������� ��� ���������� ���������� ������� ������
// �������� ��������� ��� ������ ���������� � ������� - ��� Py_CUSTOM_BLOCK_THREADS (�� ���� ������������ � ������������� ����������)
// �������� � boost Py_BLOCK_THREADS � Py_UNBLOCK_THREADS
#define Py_CUSTOM_BLOCK_THREADS        if(_custom_save!=nullptr){PyEval_RestoreThread(_custom_save);\
                                                                _custom_save=nullptr;}

#define Py_CUSTOM_UNBLOCK_THREADS      if(_custom_save==nullptr){_custom_save = PyEval_SaveThread();}


namespace RDK {

class TPyComponent: virtual public RDK::UNet
{
public: // ��������
/// ��� ����� ������������ �������, �������� ������������ ����� ������������
ULProperty<std::string, TPyComponent> PythonScriptFileName;

/// Deprecated: PythonScriptPath == PythonScriptFileName
//ULProperty<std::string, TPyComponent> PythonScriptPath;

/// ��� �����-������
ULProperty<std::string, TPyComponent> PythonModuleName;

/// ��� �����-������
ULProperty<std::string, TPyComponent> PythonClassName;

/// ������������ ���� � ����� "��� ����". ��� �� ���������, ���� ����������� ��� ������ ��������
ULProperty<bool, TPyComponent> UseFullPath;

protected: // ��������� ����������
//��������� ������
//����� ����� ���������/�������� ���������� ������� ������
static PyThreadState *_custom_save;

/// ���� ��������� ��� �������� ������������� ���������� ������
bool PythonInitialized;

/// ��������� 1
boost::python::object IntegrationInterface;

/// ��������� 2
boost::python::object IntegrationInterfaceInstance;

/// ������ ���� �� ����� �������
std::string FullPythonScriptFileName;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TPyComponent(void);
virtual ~TPyComponent(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
bool SetPythonScriptFileName(const std::string& path);
//bool SetPythonClassifierScriptPath(const std::string& path);
bool SetPythonModuleName(const std::string& path);
bool SetPythonClassName(const std::string& path);
// ---------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
virtual void PythonInitialize(void);

/// ����������� ���������� true ���� ������� ����������
/// � false � ������ �������!
virtual bool APythonInitialize(void)=0;

// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ADefault(void);
virtual bool APyDefault(void)=0;

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool ABuild(void);
virtual bool APyBuild(void)=0;

// ����� �������� ����� ��� ������ ��������
virtual bool AReset(void);
virtual bool APyReset(void)=0;

// ��������� ������ ����� �������
virtual bool ABeforeCalculate(void);

// --------------------------
};

}

#endif
