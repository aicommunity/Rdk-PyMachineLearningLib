#ifndef RDK_TPyDetectorTrainerH
#define RDK_TPyDetectorTrainerH

#include "TPyBaseTrainer.h"




namespace RDK {

class TPyDetectorTrainer: public TPyBaseTrainer
{
public: //   

///         : "xml_main_dirs", "xml_txt_splits", "xml_txt_main_lists", "xml_test_dirs", "xml_txt_test_splits"
UProperty<std::string, TPyDetectorTrainer, ptPubParameter> DatasetType;

///        
UProperty<std::string, TPyDetectorTrainer, ptPubParameter> Config;

///        /pred  
/// xml        
UProperty<bool, TPyDetectorTrainer, ptPubParameter> SavePredicted;

///    /images        .
///  :
///     False   -              -  0
///     All     -             -  -1
///     n       -  n  -   n
///    n,    n      
UProperty<int, TPyDetectorTrainer, ptPubParameter> Visualize;

///             visualize
/// (    )        ,
///        
UProperty<bool, TPyDetectorTrainer, ptPubParameter> PaintGt;

//  
///       
UProperty<std::vector<std::string>, TPyDetectorTrainer, ptPubState> LossNames;

///       
UProperty<std::vector<double>, TPyDetectorTrainer, ptPubState> TrainLosses;

///       
UProperty<std::vector<double>, TPyDetectorTrainer, ptPubState> ValLosses;

protected: //  



public: // 
// --------------------------
//   
// --------------------------
TPyDetectorTrainer(void);
virtual ~TPyDetectorTrainer(void);
// --------------------------

// --------------------------
//    
// --------------------------
//         
virtual TPyDetectorTrainer* New(void);
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



