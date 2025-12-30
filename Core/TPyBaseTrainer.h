#ifndef RDK_TPyBaseTrainerH
#define RDK_TPyBaseTrainerH

#include "TPyComponent.h"


namespace RDK {

///         
class TPyBaseTrainer: public TPyComponent
{
public: // 

//     
///      (   )
UProperty<std::vector<std::string>, TPyBaseTrainer> TrainDataDir;

/// ,    ,   , ,    .
UProperty<std::string, TPyBaseTrainer, ptPubParameter> WorkingDir;

///    
UProperty<std::string, TPyBaseTrainer, ptPubParameter> ArchitectureName;

///    [percent_train, percent_val, percent_test]
///     (   train val test,   [0.7, 0.2, 0.1])
UProperty<std::vector<int>, TPyBaseTrainer> SplitRatio;

///     
UProperty<bool, TPyBaseTrainer, ptPubParameter> SaveSplits;

/// -   
UProperty<int, TPyBaseTrainer, ptPubParameter> Epochs;

///   (  )
UProperty<std::string, TPyBaseTrainer, ptPubParameter> Weights;

/// ,   patience
/// ( ,         ,   ) 
UProperty<int, TPyBaseTrainer, ptPubParameter> EarlyStop;

///    (    tf 1.* - n ) (   tf 2.* - n )
UProperty<int, TPyBaseTrainer, ptPubParameter> SavingInterval;

/// C       val_loss
UProperty<bool, TPyBaseTrainer, ptPubParameter> SaveBestOnly;

//    ( )
///   
//    StartTraining=true  TrainingInProgress=0
UProperty<bool, TPyBaseTrainer, ptPubParameter> StartTraining;

///   
//     ,     ,   
UProperty<bool, TPyBaseTrainer, ptPubParameter> StopTraining;

///    (  )
UProperty<bool, TPyBaseTrainer, ptPubParameter> StopNow;

///  
// -1 -  (   )     (stop_training, stop_now)).
//       0
// 0 -    (  )
// 1 - 
// 2 -   
// 3 -   .      0
UProperty<int, TPyBaseTrainer, ptPubState> TrainingStatus;

///    /  ..
// true  -  
// false -   
UProperty<bool, TPyBaseTrainer, ptPubState> ThreadIsAlive;


//  
///  
UProperty<int, TPyBaseTrainer, ptPubState> Epoch;

///  
UProperty<float, TPyBaseTrainer, ptPubState> Progress;


protected: //  



public: // 
// --------------------------
//   
// --------------------------
TPyBaseTrainer(void);
virtual ~TPyBaseTrainer(void);
// --------------------------

// --------------------------

// --------------------------
//    
// --------------------------
protected:
virtual bool APythonInitialize(void);

//        
virtual bool APyDefault(void);

//     
//   
//    Reset()   Ready  true
//    
virtual bool APyBuild(void);

//      
virtual bool APyReset(void) final;

//    
virtual bool ACalculate(void);


//      python-
virtual bool CheckInputParameters() = 0;
// --------------------------
};


}

#endif



