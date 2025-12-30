#ifndef RDK_TPythonIntegrationH
#define RDK_TPythonIntegrationH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>

namespace RDK {

class TPythonIntegration: public RDK::UNet
{
public: // 
/// 
UProperty<int,TPythonIntegration, ptPubParameter> Param1;

///  
UProperty<UBitmap,TPythonIntegration, ptPubParameter> InputImage;

///  
UProperty<std::string,TPythonIntegration, ptPubParameter> InputFile;
///     
UProperty<MDMatrix<int>,TPythonIntegration> Detections;

///     
UProperty<MDMatrix<int>,TPythonIntegration> DetectionClass;

///     
UProperty<MDMatrix<double>,TPythonIntegration> DetectionReliability;

UProperty<UBitmap,TPythonIntegration, ptPubParameter> DebugImage;

boost::python::object IntegrationInterface;
boost::python::object IntegrationInterfaceInstance;

protected: //  

UGraphics Graph;

public: // 
// --------------------------
//   
// --------------------------
TPythonIntegration(void);
virtual ~TPythonIntegration(void);
// --------------------------

// ---------------------
//   
// ---------------------
// ---------------------

// ---------------------
//    
// ---------------------
// ---------------------

// --------------------------
//    
// --------------------------
//         
virtual TPythonIntegration* New(void);
// --------------------------

// --------------------------
//    
// --------------------------
protected:
virtual void AInit(void);
virtual void AUnInit(void);

//        
virtual bool ADefault(void);

//     
//   
//    Reset()   Ready  true
//    
virtual bool ABuild(void);

//      
virtual bool AReset(void);

//    
virtual bool ACalculate(void);
// --------------------------
};


}

#endif



