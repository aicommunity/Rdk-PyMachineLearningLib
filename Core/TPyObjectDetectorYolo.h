#ifndef RDK_TPyObjectDetectorYoloH
#define RDK_TPyObjectDetectorYoloH

#include "TPyObjectDetector.h"

namespace RDK {

#define YOLOV2_INITTYPE 2
#define YOLOV3_INITTYPE 3

class TPyObjectDetectorYolo: public TPyObjectDetector
{
public: // 
/// : 2 - YOLOV2
///                   3 - YOLOV3
UProperty<int,TPyObjectDetectorYolo, ptPubParameter> InitializationTypeYOLO;

///    
UProperty<std::string,TPyObjectDetectorYolo, ptPubParameter> ConfigPathYOLO;

///    
UProperty<std::string,TPyObjectDetectorYolo, ptPubParameter> WeightsPathYOLO;

protected: //  

std::vector<std::string> ClassedList;

public: // 
// --------------------------
//   
// --------------------------
TPyObjectDetectorYolo(void);
virtual ~TPyObjectDetectorYolo(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual TPyObjectDetectorYolo* New(void);
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



