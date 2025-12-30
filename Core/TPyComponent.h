#ifndef RDK_TPyComponentH
#define RDK_TPyComponentH

#include "TPythonIntegrationInclude.h"
#include "TPythonIntegrationUtil.h"



namespace RDK {

class TPyComponent: virtual public RDK::UNet
{
public: // 
///    ,    
UProperty<std::string, TPyComponent, ptPubParameter> PythonScriptFileName;

/// Deprecated: PythonScriptPath == PythonScriptFileName
//UProperty<std::string, TPyComponent, ptPubParameter> PythonScriptPath;

///  -
UProperty<std::string, TPyComponent, ptPubParameter> PythonModuleName;

///  -
UProperty<std::string, TPyComponent, ptPubParameter> PythonClassName;

///     " ".   ,     
UProperty<bool, TPyComponent, ptPubParameter> UseFullPath;

protected: //  

//  / GIL
PyGILState_STATE gil_state;

///       
bool PythonInitialized;

///  1
boost::python::object* IntegrationInterface;

///  2
boost::python::object* IntegrationInterfaceInstance;

///     
std::string FullPythonScriptFileName;


public: // 
// --------------------------
//   
// --------------------------
TPyComponent(void);
virtual ~TPyComponent(void);
// --------------------------

// ---------------------
//   
// ---------------------
bool SetPythonScriptFileName(const std::string& path);
//bool SetPythonClassifierScriptPath(const std::string& path);
bool SetPythonModuleName(const std::string& path);
bool SetPythonClassName(const std::string& path);
// ---------------------

// --------------------------
//    
// --------------------------
protected:
virtual void PythonInitialize(void);

///   true   
///  false   !
virtual bool APythonInitialize(void)=0;

//        
virtual bool ADefault(void);
virtual bool APyDefault(void)=0;

//     
//   
//    Reset()   Ready  true
//    
virtual bool ABuild(void);
virtual bool APyBuild(void)=0;

//      
virtual bool AReset(void);
virtual bool APyReset(void)=0;

//    
virtual bool ABeforeCalculate(void);

// --------------------------
};

}

#endif
