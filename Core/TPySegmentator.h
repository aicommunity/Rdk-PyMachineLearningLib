#ifndef RDK_TPySegmentatorH
#define RDK_TPySegmentatorH

#include "TPyComponent.h"
#include "../../Rdk-CRLib/Core/USegmentatorBase.h"

namespace RDK {

#define YOLOV2_INITTYPE 2
#define YOLOV3_INITTYPE 3

class TPySegmentator: public USegmentatorBase, public TPyComponent
{
public: // 


///  ,   ,    
/// ubmRGB24=3 -  
/// umbY8=400 - - 
UProperty<int,TPySegmentator, ptPubParameter> ImageColorModel;


protected: //  
UGraphics Graph;


public: // 
// --------------------------
//   
// --------------------------
TPySegmentator(void);
virtual ~TPySegmentator(void);
// --------------------------

// --------------------------
//    
// --------------------------
protected:

//        
virtual bool APyDefault(void);
virtual bool APyDefault2(void);

//     
//   
//    Reset()   Ready  true
//    
virtual bool APyBuild(void);
virtual bool APyBuild2(void);

//      
virtual bool APyReset(void);
virtual bool APyReset2(void);

//    
virtual bool Inference(UBitmap &bmp, UBitmap &mask);
// --------------------------
};


}

#endif



