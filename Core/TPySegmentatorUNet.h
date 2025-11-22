#ifndef RDK_TPySegmentatorUNetH
#define RDK_TPySegmentatorUNetH

#include "TPySegmentator.h"

namespace RDK {

class TPySegmentatorUNet: public TPySegmentator
{
public: // 

///   
UProperty<std::string,TPySegmentatorUNet, ptPubParameter> WeightsPath;

///        
UProperty<double,TPySegmentatorUNet, ptPubParameter> RespondThreshold;

protected: //  
public: // 
// --------------------------
//   
// --------------------------
TPySegmentatorUNet(void);
virtual ~TPySegmentatorUNet(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual TPySegmentatorUNet* New(void);
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
bool Inference(UBitmap &bmp, UBitmap &mask);
// --------------------------
};


}

#endif



