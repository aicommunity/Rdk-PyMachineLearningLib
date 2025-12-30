#ifndef RDK_TPyObjectDetectorSqueezeDetH
#define RDK_TPyObjectDetectorSqueezeDetH

#include "TPyObjectDetector.h"

namespace RDK {

class TPyObjectDetectorSqueezeDet: public TPyObjectDetector
{
public: // 

///   
UProperty<std::string,TPyObjectDetectorSqueezeDet, ptPubParameter> ConfigPath;

///   
UProperty<std::string,TPyObjectDetectorSqueezeDet, ptPubParameter> WeightsPath;

protected: //  

std::vector<std::string> ClassedList;

public: // 
// --------------------------
//   
// --------------------------
TPyObjectDetectorSqueezeDet(void);
virtual ~TPyObjectDetectorSqueezeDet(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual TPyObjectDetectorSqueezeDet* New(void);
// --------------------------


// --------------------------
//    
// --------------------------
protected:
bool APythonInitialize(void);


//        
virtual bool APyDefault2(void);

//     
//   
//    Reset()   Ready  true
//    
virtual bool APyBuild2(void);

//      
virtual bool APyReset2(void);

//  
bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
// --------------------------
};


}

#endif



