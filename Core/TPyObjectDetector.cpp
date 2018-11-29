#define NO_IMPORT_ARRAY

#ifndef RDK_TPyObjectDetectorCPP
#define RDK_TPyObjectDetectorCPP

#include "TPyObjectDetector.h"
#include <iostream>

namespace RDK {
/*
#if (PY_VERSION_HEX >= 0x03000000)
    void *init_py_pyubclsfr() {
#else
    void init_py_pyubclsfr(){
#endif
        if(Py_IsInitialized())
            return NUMPY_IMPORT_ARRAY_RETVAL;
        Py_Initialize();

        import_array();
        np::initialize();
        return NUMPY_IMPORT_ARRAY_RETVAL;
    }*/

// ������
// --------------------------
// ������������ � �����������
// --------------------------  //DetectionClass("DetectionClass",this),
TPyObjectDetector::TPyObjectDetector(void)
: InputImage("InputImage",this),
  Initialized(false),
  OutputObjects("OutputObjects",this),
  ImageColorModel("ImageColorModel",this),
  //ModelPathYOLO("ModelPathYOLO",this),
  //AnchorsPathYOLO("AnchorsPathYOLO",this),
  //ClassesPathYOLO("ClassesPathYOLO",this),
  //TargetClassesYOLO("TargetClassesYOLO",this),
  //ChangeClassesYOLO("ChangeClassesYOLO",this),
  OutputImage("OutputImage",this),
  InitializationTypeYOLO("InitializationTypeYOLO",this),
  ConfigPathYOLO("ConfigPathYOLO",this),
  WeightsPathYOLO("WeightsPathYOLO",this)
  //LoadTargetClassesYOLO("LoadTargetClassesYOLO",this)
  //OutputConfidences("OutputConfidences", this)
  //PythonScriptFileName("PythonScriptFileName",this)
{
    AddLookupProperty("PythonScriptPath",ptPubParameter, new UVProperty<std::string,TPyObjectDetector>(this,
                 &TPyObjectDetector::SetPythonClassifierScriptPath,&TPyObjectDetector::GetPythonClassifierScriptPath));

    /*AddLookupProperty("NumTargetClassesYOLO",ptPubParameter, new UVProperty<int,TPyObjectDetector>(this,
                 &TPyObjectDetector::SetNumTargetClassesYOLO,&TPyObjectDetector::GetNumTargetClassesYOLO));

    AddLookupProperty("NumChangeClassesYOLO",ptPubParameter, new UVProperty<int,TPyObjectDetector>(this,
                 &TPyObjectDetector::SetNumChangeClassesYOLO,&TPyObjectDetector::GetNumChangeClassesYOLO));
*/
}

bool TPyObjectDetector::SetPythonClassifierScriptPath(const std::string& path)
{
    PythonScriptFileName = path;
    Initialized=false;
    return true;
}
const std::string & TPyObjectDetector::GetPythonClassifierScriptPath(void) const
{
    return PythonScriptFileName;
}
/*
bool TPyObjectDetector::SetNumTargetClassesYOLO(const int& num)
{
    NumTargetClassesYOLO = num;
    TargetClassesYOLO->resize(num);
    return true;
}
const int& TPyObjectDetector::GetNumTargetClassesYOLO(void) const
{
    return NumTargetClassesYOLO;
}

bool TPyObjectDetector::SetNumChangeClassesYOLO(const int& num)
{
    NumChangeClassesYOLO = num;
    ChangeClassesYOLO->resize(num);
    return true;
}
const int& TPyObjectDetector::GetNumChangeClassesYOLO(void) const
{
    return NumChangeClassesYOLO;
}
*/

TPyObjectDetector::~TPyObjectDetector(void)
{
}
// --------------------------


// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
TPyObjectDetector* TPyObjectDetector::New(void)
{
 return new TPyObjectDetector;
}
// --------------------------

void TPyObjectDetector::AInit(void)
{
    if(!Initialized)
    {
       if(!Initialize())
           return;
    }
}

// --------------------------
// ������� ������ ���������� ������
// --------------------------
bool TPyObjectDetector::Initialize(void)
{
return true;
    try
    {
        LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("Python init started..."));
        init_py();
        py::to_python_converter<cv::Mat, pbcvt::matToNDArrayBoostConverter>();
        py::to_python_converter<RDK::UBitmap, pbcvt::uBitmapToNDArrayBoostConverter>();
        py::object MainModule = py::import("__main__");  // ����������� main-scope, ��. https://docs.python.org/3/library/__main__.html
        py::object MainNamespace = MainModule.attr("__dict__");  // ��������� ������� ����

        // �������� ���� �� ����� � ����������� ������� ����
        std::string s = this->GetEnvironment()->GetCurrentDataDir()+PythonScriptFileName;
        py::object DetectorInterfaceModule = import("detector_interface",s,MainNamespace);
        // ��������� ������������ ������, ����� ������� ������������ ����������� ����� � ����������� ������
        // TODO: ����� �� ����� ����� �� �������
        IntegrationInterface = DetectorInterfaceModule.attr("DetectorEmbeddingInterface");
        if(!IntegrationInterface.is_none())
            IntegrationInterfaceInstance = IntegrationInterface(); ///home/arnold/.virtualenvs/cv

        //boost::python::object rand_mod = boost::python::import("random");
        //boost::python::object rand_func = rand_mod.attr("random");
        //boost::python::object rand2 = rand_func();
        //std::cout << boost::python::extract<int>(rand2) << std::endl;

        /*
        py::list target_classes = py::list();
        for(int i=0; i<TargetClassesYOLO->size(); i++)
        {
            target_classes.insert(i, (*TargetClassesYOLO)[i]);
        }

        py::list change_classes = py::list();
        if(ChangeClassesYOLO->size()>0)
        {
            for(int i=0; i<ChangeClassesYOLO->size(); i++)
            {
                change_classes.insert(i, (*ChangeClassesYOLO)[i]);
            }
        }
        */

        py::object initialize;
        switch(InitializationTypeYOLO)
        {
            case YOLOV2_INITTYPE:
                initialize = IntegrationInterfaceInstance.attr("initialize_config")(*ConfigPathYOLO, *WeightsPathYOLO);
            break;
            case YOLOV3_INITTYPE:
                initialize = IntegrationInterfaceInstance.attr("initialize_config")(*ConfigPathYOLO);
            break;
        default:
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Chosen initialization type not supported by selected detector interface file"));
            Initialized = false;
            return false;
            break;

        }



        if(!initialize.is_none())
        {
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init success"));
            Initialized = true;
            return true;
        }
        else
        {
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Chosen initialization type not supported by selected detector interface file"));
            Initialized = false;
            return false;
        }
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init fail: ")+perrorStr);
        Initialized=false;
        return false;
    }
    LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("...Python init finished successful!"));
    return true;
}

void TPyObjectDetector::AUnInit(void)
{
 Initialized=false;
}

// �������������� �������� �� ��������� � ����� �������� �����
bool TPyObjectDetector::ADefault(void)
{
 Initialized=false;
 //InitializationTypeYOLO = 1;
 return true;
}
// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TPyObjectDetector::ABuild(void)
{
 return true;
}

// ����� �������� ����� ��� ������ ��������
bool TPyObjectDetector::AReset(void)
{
 //Initialized = false;
 return true;
}

// ��������� ������ ����� �������
bool TPyObjectDetector::ACalculate(void)
{
return true;
// if(!Initialized)
// {
//    if(!Initialize())
//        return true;
// }

 /*if(LoadTargetClassesYOLO)
 {
     if(ClassedList.size()==0)
     {
        std::ifstream fl;
        fl.open(ClassesPathYOLO);
        if(!fl.is_open())
        {
            std::string s = this->GetEnvironment()->GetCurrentDataDir()+*ClassesPathYOLO;
            fl.open(s);
        }

        if(fl.is_open())
        {
            while(!fl.eof())
            {
                std::string str;
                std::getline(fl, str);
                ClassedList.insert(ClassedList.end(), str);
            }
        }
     }
 }
*/

 if(!InputImage.IsConnected())
  return true;



 /*if(ImageColorModel!=bmp.GetColorModel())
 {
     LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("Incorrect input image color model. Need "+sntoa(*ImageColorModel)+" got: ")+sntoa(bmp.GetColorModel()));
     return true;
 }*/

 OutputImage->SetColorModel(ubmRGB24,false);
 InputImage->ConvertTo(*OutputImage);

 UBitmap &bmp = *OutputImage;
 if(bmp.GetData()==NULL)
  return true;

 int w = bmp.GetWidth();
 int h = bmp.GetHeight();

 UBitmap b;
 b.SetRes(w, h, bmp.GetColorModel());
 bmp.CopyTo(0,0,b);

 Graph.SetCanvas(OutputImage);

 /// ��� �������
 std::vector<std::vector<double> > result;
 try
 {
  //import_array();
  py::object retval = IntegrationInterfaceInstance.attr("detect")(b);

  //std::vector<float> res = boost::python::extract<std::vector<float> >(retval);
  np::ndarray ndarr = boost::python::extract< np::ndarray  >(retval);
  int dms = ndarr.get_nd();

  if(dms>2)
  {
      LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetector error: Returned array with incorrect dimensions"));
      return true;
  }

  const Py_intptr_t* shp = ndarr.get_shape();
  long height = shp[0];
  long width  = shp[1];

  if(width!=6)
      LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetector error: Output matrix WIDTH!=6!!! must be in [x y w h conf class] format!!!"));

  const Py_intptr_t *strides = ndarr.get_strides();
  long str0 = ndarr.strides(0);
  long str1 = ndarr.strides(1);


  result.resize(height);

  double *data = reinterpret_cast<double*>(ndarr.get_data());
  for(int y=0; y<height; y++)
  {
      result[y].resize(width);
      for(int x=0; x<width;x++)
      {
          double val = data[y*width+x];
          result[y][x] = val;
      }
  }

  /*
  OutputObjects->Resize(height, width);

  for(int y=0; y<height; y++)
  {
      result[y].resize(width);
      for(int x=0; x<width;x++)
      {
          (*OutputObjects)(y, x) = result[y][x];
      }
  }
  */

  UAFont *class_font=GetFont("Tahoma",14);

  for(int i=0; i<result.size(); i++)
  {
      int xmin, ymin, xmax, ymax;
      xmin = (int)(result[i][0]);
      ymin = (int)(result[i][1]);
      xmax = (int)(result[i][2]);
      ymax = (int)(result[i][3]);


      Graph.SetPenColor(0x00FF00);
      Graph.Rect(xmin, ymin, xmax, ymax);

      double conf = result[i][4];
      int cls = static_cast<int>(result[i][5]);

      std::stringstream ss;
      ss<<"[";
      if(conf>0)
      {
          ss<<"P="<<conf;
      }

      /*if(ClassedList.size()>cls)
      {
          ss<<"; C="<<ClassedList[cls].c_str();
      }
      else
      {*/
          ss<<"; C="<<cls;
      //}
      ss<<"]";

      if(class_font)
      {
        Graph.SetFont(class_font);
        Graph.Text(ss.str(),xmin, ymax+3);
      }
  }

  //int k=1+21;
  //���� �� ��������� �� ������ �� ���������� � ������ ������!
  //if(result.size()!=NumClasses)
  //{
  //    LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetector error: NumClasses not equals to returned confidences count"));
  //    return true;
  //}

  //int max_id = -1;
  //double max_conf = -100;
  //for(int k=0; k<result.size(); k++)
  //{
  //    (*OutputConfidences)(i, k) = result[k];
  //    if(result[k]>max_conf)
  //    {
  //        max_conf = result[k];
  //        max_id = k;
  //    }
  //}
  //(*OutputClasses)[i] = max_id;
 }
 catch (py::error_already_set const &)
 {
  std::string perrorStr = parse_python_exception();
  LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetector error: ")+perrorStr);
 }

  /*
 if(InputImages->size()>0)
 {
     OutputClasses->clear();
     OutputClasses->resize(InputImages->size(), -1);
     OutputConfidences->Resize(InputImages->size(), NumClasses);
     for(int i=0; i<InputImages->size(); i++)
     {
         UBitmap &bmp = (*InputImages)[i];
         if(bmp.GetData()==NULL)
            continue;

         if(ImageColorModel!=bmp.GetColorModel())
         {
             LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("Incorrect image ["+sntoa(i)+"] color model. Need "+sntoa(*ImageColorModel)+" got: ")+sntoa(bmp.GetColorModel()));
             return true;
         }

         int w = bmp.GetWidth();
         int h = bmp.GetHeight();

         UBitmap b;
         b.SetRes(w, h, bmp.GetColorModel());
         bmp.CopyTo(0,0,b);


         //RDK::SaveBitmapToFile("/home/ivan/testB.bmp", b);

         int object_cls = -1;
         /// ��� �������
         try
         {
 //         import_array();
          py::object retval = IntegrationInterfaceInstance.attr("classify")(b);

          //std::vector<float> res = boost::python::extract<std::vector<float> >(retval);
          boost::numpy::ndarray ndarr = boost::python::extract< boost::numpy::ndarray  >(retval);
          int dms = ndarr.get_nd();

          if(dms>2)
          {
              LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetector error: Returned array with incorrect dimensions"));
              return true;
          }

          const long* shp = ndarr.get_shape();
          long height = shp[0];
          long width  = shp[1];

          const long *strides = ndarr.get_strides();
          long str0 = ndarr.strides(0);
          long str1 = ndarr.strides(1);

          std::vector<float> result;
          float *data = reinterpret_cast<float*>(ndarr.get_data());
          for(int y=0; y<height; y++)
          {
              for(int x=0; x<width;x++)
              {
                  float val = data[y*width+x];
                  result.push_back(val);
              }
          }
          //���� �� ��������� �� ������ �� ���������� � ������ ������!
          if(result.size()!=NumClasses)
          {
              LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetector error: NumClasses not equals to returned confidences count"));
              return true;
          }

          int max_id = -1;
          double max_conf = -100;
          for(int k=0; k<result.size(); k++)
          {
              (*OutputConfidences)(i, k) = result[k];
              if(result[k]>max_conf)
              {
                  max_conf = result[k];
                  max_id = k;
              }
          }
          (*OutputClasses)[i] = max_id;

         }
         catch (py::error_already_set const &)
         {
          std::string perrorStr = parse_python_exception();
          LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetector error: ")+perrorStr);
         } 
     }
 }

 //������ �� ���� ���������
 /*for(int i = 0; i < AggrRectsMatrix->GetRows(); ++i)
 {
  int object_cls = -1;
  /// ��� �������
  try
  {
   py::object retval = IntegrationInterfaceInstance.attr("classify")(obj_rect);

   object_cls = boost::python::extract<int>(retval);
  }
  catch (py::error_already_set const &)
  {
   std::string perrorStr = parse_python_exception();
   LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python error: ")+perrorStr);
  }
  if(object_cls==0)
  {
   Graph.SetPenColor(0x0000FF);
  }
  else if(object_cls==1)
  {
   Graph.SetPenColor(0x00FF00);
  }
 }*/
/*

 //LogMessageEx(RDK_EX_INFO,__FUNCTION__,"test");
 //MDMatrix<int> &detections=*Detections;
 //MDMatrix<int> &detection_class=*DetectionClass;
 //MDMatrix<double> &detection_reliability=*DetectionReliability;


 /// � ������ ������������ ���������� �� �������



 int num_objects(0);
 for(int i=0;i<num_objects;i++)
 {
  int x(0),y(0),width(0),height(0);
  int class_id(0);
  double reliability(0.0);

  detections.Resize(num_objects,4);
  detection_class.Resize(num_objects,1);
  detection_reliability.Resize(num_objects,1);

  detections(i,0)=x;
  detections(i,1)=y;
  detections(i,2)=width;
  detections(i,3)=height;
  detection_class(i,0)=class_id;
  detection_reliability(i,0)=reliability;

  Graph.SetPenColor(0x0000FF);
  Graph.Rect(x,y,x+width,y+height);
 }*/

 return true;
}
// --------------------------

}
#endif



