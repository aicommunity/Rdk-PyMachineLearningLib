#ifndef RDK_TPyClassifierTrainerH
#define RDK_TPyClassifierTrainerH

#include "TPyBaseTrainer.h"



namespace RDK {

class TPyClassifierTrainer: public TPyBaseTrainer
{
public: //   

///           
UProperty<std::string, TPyClassifierTrainer, ptPubParameter> DatasetName;

///         train, val, test
///      
UProperty<bool, TPyClassifierTrainer, ptPubParameter> CopySplittedImages;

///      .
///      70, 20, 10  TestEqualVal=True,  10%    .
UProperty<bool, TPyClassifierTrainer, ptPubParameter> TestEqualVal;

///   (width, height, channels)
UProperty<std::vector<int>, TPyClassifierTrainer> ImageSize;

/// Learning Rate
UProperty<float, TPyClassifierTrainer, ptPubParameter> LearningRate;

///   batch-,  train, val  test
UProperty<std::vector<int>, TPyClassifierTrainer> BatchSizes;

///   . N 
UProperty<int, TPyClassifierTrainer, ptPubParameter> LayersToBeTrained;

/// , (),       
UProperty<std::vector<std::string>, TPyClassifierTrainer> Classes;

///     
UProperty<float, TPyBaseTrainer, ptPubState> TrainLoss;

///     
UProperty<float, TPyBaseTrainer, ptPubState> TrainAcc;

///     
UProperty<float, TPyBaseTrainer, ptPubState> ValLoss;

///     
UProperty<float, TPyBaseTrainer, ptPubState> ValAcc;

protected: //  


public: // 
// --------------------------
//   
// --------------------------
TPyClassifierTrainer(void);
virtual ~TPyClassifierTrainer(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual TPyClassifierTrainer* New(void);
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
//virtual bool APyReset(void);

//    
virtual bool ACalculate(void);


//      python-
virtual bool CheckInputParameters();
// --------------------------
};


}

#endif



