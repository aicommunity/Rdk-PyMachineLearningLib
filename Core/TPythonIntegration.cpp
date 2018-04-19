#ifndef RDK_TPythonIntegrationCPP
#define RDK_TPythonIntegrationCPP

#include "TPythonIntegration.h"

namespace RDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------
TPythonIntegration::TPythonIntegration(void)
{
}

TPythonIntegration::~TPythonIntegration(void)
{
}
// --------------------------    


// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
TPythonIntegration* TPythonIntegration::New(void)
{
 return new TPythonIntegration;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������ 
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool TPythonIntegration::ADefault(void)
{           
 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TPythonIntegration::ABuild(void)
{
 return true;
}

// ����� �������� ����� ��� ������ ��������
bool TPythonIntegration::AReset(void)
{
 return true;
}

// ��������� ������ ����� �������
bool TPythonIntegration::ACalculate(void)
{
 return true;
}
// --------------------------

}
#endif



