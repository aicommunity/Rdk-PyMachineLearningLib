#ifndef RDK_TPyPredictSortH
#define RDK_TPyPredictSortH

#include "TPyComponent.h"


namespace RDK {

class TPyPredictSort : public TPyComponent
{
public: //   

/// ,    
UProperty<std::string, TPyPredictSort, ptPubParameter> WorkingDir;

///     
UProperty<std::string, TPyPredictSort, ptPubParameter> ImagesDir;

///     
UProperty<std::string, TPyPredictSort, ptPubParameter> WeightPath;

///     
UProperty<std::string, TPyPredictSort, ptPubParameter> ConfigPath;

///        
UProperty<bool, TPyPredictSort, ptPubParameter> SortImages;

///   
UProperty<bool, TPyPredictSort, ptPubParameter> StopNow;

///    
// true  -  
// false -   
UProperty<bool, TPyPredictSort, ptPubState> ThreadIsAlive;

//    ( )
///   
//    StartTraining=true  PredictStatus=0
UProperty<bool, TPyPredictSort, ptPubParameter> StartPredict;

///   
UProperty<bool, TPyPredictSort, ptPubState> PredictionEnded;

///  
// -1 -  (   )     (stop_training, stop_now)).
//       0
// 0 -    (  )
// 1 -  
// 2 -   
UProperty<int, TPyPredictSort, ptPubState> PredictStatus;

protected: //  


public: // 
// --------------------------
//   
// --------------------------
TPyPredictSort(void);
virtual ~TPyPredictSort(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual TPyPredictSort* New(void);
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



