#ifndef RDK_TPySegmenterTrainerH
#define RDK_TPySegmenterTrainerH

#include "TPyBaseTrainer.h"




namespace RDK {

class TPySegmenterTrainer: public TPyBaseTrainer
{
public: //   

    //  TrainDataDir 2 :
    //      "basedir_ann" -    
    //      "basedir_img" -    


///    
UProperty<std::string, TPySegmenterTrainer, ptPubParameter> Config;

///     : "txt_data", "split_data", "not_split_data".
UProperty<std::string, TPySegmenterTrainer, ptPubParameter> DatasetType;

///            
UProperty<std::string, TPySegmenterTrainer, ptPubParameter> DatasetName;

/// ,     , : [height,width,channels].
UProperty<std::vector<int>, TPySegmenterTrainer> InputRes;

///  
UProperty<std::vector<std::string>, TPySegmenterTrainer> Classes;

///    
UProperty<int, TPySegmenterTrainer, ptPubParameter> BatchSize;

///    
UProperty<int, TPySegmenterTrainer, ptPubParameter> BatchesInEpoch;


//  
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
TPySegmenterTrainer(void);
virtual ~TPySegmenterTrainer(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual TPySegmenterTrainer* New(void);
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



