#ifndef RDK_TPyDetPredictH
#define RDK_TPyDetPredictH

#include "TPyComponent.h"


namespace RDK {

class TPyDetPredict : public TPyComponent
{
public: //   

/// ,    
UProperty<std::string, TPyDetPredict, ptPubParameter> WorkingDir;

///     
UProperty<std::string, TPyDetPredict, ptPubParameter> ImagesDir;

///     
UProperty<std::string, TPyDetPredict, ptPubParameter> WeightPath;

///     
UProperty<std::string, TPyDetPredict, ptPubParameter> ConfigPath;

///   
UProperty<bool, TPyDetPredict, ptPubParameter> StopNow;

///    
// true  -  
// false -   
UProperty<bool, TPyDetPredict, ptPubState> ThreadIsAlive;

//    ( )
///   
//    StartTraining=true  PredictStatus=0
UProperty<bool, TPyDetPredict, ptPubParameter> StartPredict;

///  
// -1 -  (   )     (stop_training, stop_now)).
//       0
// 0 -    (  )
// 1 -  
// 2 -   
UProperty<int, TPyDetPredict, ptPubState> PredictStatus;

protected: //  


public: // 
// --------------------------
//   
// --------------------------
TPyDetPredict(void);
virtual ~TPyDetPredict(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual TPyDetPredict* New(void);
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

//    
virtual bool ACalculate(void);


//      python-
virtual bool CheckInputParameters();
// --------------------------
};


}

#endif



