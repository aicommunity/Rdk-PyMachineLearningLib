#ifndef RDK_TPyUBitmapClassifierH
#define RDK_TPyUBitmapClassifierH

#include "TPyComponent.h"
#include "../../Rdk-CRLib/Core/UClassifierBase.h"

namespace RDK {

class TPyUBitmapClassifier: public UClassifierBase, public TPyComponent
{
public: // 
///  ,   ,    
/// ubmRGB24=3 -  
/// umbY8=400 - - 
UProperty<int,TPyUBitmapClassifier, ptPubParameter> ImageColorModel;

///  (  )    
UProperty<std::string,TPyUBitmapClassifier, ptPubParameter> WeightsPath;

///       
///   -   ..   initialize_weights(self, weights_path)
///      - (. True)     
UProperty<bool,TPyUBitmapClassifier, ptPubParameter> UseWeightsPath;

///        
UProperty<bool,TPyUBitmapClassifier, ptPubParameter> UseRelativeWeightsPath;

protected: //  

cv::Mat ProcessedMat;

UGraphics Graph;
UBitmap Canvas;

public: // 
// --------------------------
//   
// --------------------------
TPyUBitmapClassifier(void);
virtual ~TPyUBitmapClassifier(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual TPyUBitmapClassifier* New(void);
// --------------------------

// --------------------------
//    
// --------------------------
protected:
bool APythonInitialize(void);

//        
virtual bool APyDefault(void);

//     
//   
//    Reset()   Ready  true
//    
virtual bool APyBuild(void);

//      
virtual bool APyReset(void);


///   
virtual bool ClassifyBitmap(UBitmap &bmp, MDVector<double> &output_confidences, double conf_thresh, int &class_id, bool &is_classified);
// --------------------------
};


}

#endif



