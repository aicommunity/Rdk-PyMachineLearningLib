#ifndef RDK_TPyObjectDetectorH
#define RDK_TPyObjectDetectorH

#include "TPyComponent.h"
#include "../../Rdk-CRLib/Core/UDetectorBase.h"
namespace RDK {

#define YOLOV2_INITTYPE 2
#define YOLOV3_INITTYPE 3

class TPyObjectDetector: public UDetectorBase, public TPyComponent
{
public: // 
///  ,   ,    
/// ubmRGB24=3 -  
/// umbY8=400 - - 
UProperty<int,TPyObjectDetector, ptPubParameter> ImageColorModel;

protected: //  


public: // 
// --------------------------
//   
// --------------------------
TPyObjectDetector(void);
virtual ~TPyObjectDetector(void);
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
virtual bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
// --------------------------
};


}

#endif



