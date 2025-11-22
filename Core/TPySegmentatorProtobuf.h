#ifndef RDK_TPySegmentatorProtobufH
#define RDK_TPySegmentatorProtobufH

#include "TPySegmentator.h"

namespace RDK {

class TPySegmentatorProtobuf: public TPySegmentator
{
public: // 

///   
UProperty<std::string,TPySegmentatorProtobuf, ptPubParameter> JSONPath;

///   
UProperty<std::string,TPySegmentatorProtobuf, ptPubParameter> ProtobufPath;

protected: //  
public: // 
// --------------------------
//   
// --------------------------
TPySegmentatorProtobuf(void);
virtual ~TPySegmentatorProtobuf(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual TPySegmentatorProtobuf* New(void);
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



