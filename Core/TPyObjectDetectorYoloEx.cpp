#define NO_IMPORT_ARRAY

#ifndef RDK_TPyObjectDetectorYoloExCPP
#define RDK_TPyObjectDetectorYoloExCPP

#include "TPyObjectDetectorYoloEx.h"
#include <iostream>

namespace RDK {


// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyObjectDetectorYoloEx::TPyObjectDetectorYoloEx(void)
: ModelPathYOLO("ModelPathYOLO",this),
  AnchorsPathYOLO("AnchorsPathYOLO",this),
  ClassesPathYOLO("ClassesPathYOLO",this),
  TargetClassesYOLO("TargetClassesYOLO",this),
  ChangeClassesYOLO("ChangeClassesYOLO",this),
  LoadTargetClassesYOLO("LoadTargetClassesYOLO",this),
  NumTargetClassesYOLO("NumTargetClassesYOLO",this,&TPyObjectDetectorYoloEx::SetNumTargetClassesYOLO),
  NumChangeClassesYOLO("NumChangeClassesYOLO",this,&TPyObjectDetectorYoloEx::SetNumChangeClassesYOLO)
{
}

bool TPyObjectDetectorYoloEx::SetNumTargetClassesYOLO(const int& num)
{
    TargetClassesYOLO->resize(num);
    return true;
}

bool TPyObjectDetectorYoloEx::SetNumChangeClassesYOLO(const int& num)
{
    ChangeClassesYOLO->resize(num);
    return true;
}

TPyObjectDetectorYoloEx::~TPyObjectDetectorYoloEx(void)
{
}
// --------------------------


// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
TPyObjectDetectorYoloEx* TPyObjectDetectorYoloEx::New(void)
{
 return new TPyObjectDetectorYoloEx;
}
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
bool TPyObjectDetectorYoloEx::APythonInitialize(void)
{
    try
    {
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

        if(!UseFullPath)
        {
            *ModelPathYOLO = GetEnvironment()->GetCurrentDataDir()+*ModelPathYOLO;
            if(*AnchorsPathYOLO!="")
            {
                *AnchorsPathYOLO = GetEnvironment()->GetCurrentDataDir()+*AnchorsPathYOLO;
            }
            if(*ClassesPathYOLO!="")
            {
                *ClassesPathYOLO = GetEnvironment()->GetCurrentDataDir()+*ClassesPathYOLO;
            }
        }

        py::object initialize = IntegrationInterfaceInstance.attr("initialize_predictor")(*ModelPathYOLO, *AnchorsPathYOLO, *ClassesPathYOLO, target_classes, change_classes);
        if(!initialize.is_none())
        {
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python init success"));
            return true;
        }
        else
        {
            LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Chosen initialization type not supported by selected detector interface file"));
            return false;
        }
    }
    catch (py::error_already_set const &)
    {
        std::string perrorStr = parse_python_exception();
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("Python init fail: ")+perrorStr);
        return false;
    }
    catch(...)
    {
        LogMessageEx(RDK_EX_ERROR,__FUNCTION__,std::string("Python init fail: Undandled exception"));
        return false;
    }
    LogMessageEx(RDK_EX_INFO,__FUNCTION__,std::string("...Python init finished successful!"));
    return true;
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TPyObjectDetectorYoloEx::APyDefault2(void)
{
 NumTargetClassesYOLO=0;
 NumChangeClassesYOLO=0;
 return true;
}
// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TPyObjectDetectorYoloEx::APyBuild2(void)
{

 if(LoadTargetClassesYOLO)
 {
     if(ClassedList.size()==0)
     {
        std::ifstream fl;
        fl.open(ClassesPathYOLO);
        if(!fl.is_open())
        {
            std::string s = GetEnvironment()->GetCurrentDataDir()+*ClassesPathYOLO;
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

 return true;
}

// Сброс процесса счета без потери настроек
bool TPyObjectDetectorYoloEx::APyReset2(void)
{
 return true;
}

// Выполняет обнаружение
bool TPyObjectDetectorYoloEx::Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities)
{
    if(!PythonInitialized)
        return false;
 /// Тут считаем
// std::vector<std::vector<double> > result;
 try
 {
  py::object retval = IntegrationInterfaceInstance.attr("detect")(bmp);

  np::ndarray ndarr = boost::python::extract< np::ndarray  >(retval);
  int dms = ndarr.get_nd();

  if(dms>2)
  {
   LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetectorYoloEx error: Returned array with incorrect dimensions"));
   return false;
  }

  const Py_intptr_t* shp = ndarr.get_shape();
  long height = shp[0];
  long width  = shp[1];

  if(height>0 && width != 6)
  {
 //  LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TPyObjectDetectorYoloEx error: Matrix height and width is incorrect"));
   return false;
  }

  const Py_intptr_t *strides = ndarr.get_strides();
  long str0 = ndarr.strides(0);
  long str1 = ndarr.strides(1);


//  result.resize(height);

  double *data = reinterpret_cast<double*>(ndarr.get_data());
  output_rects.Resize(height,4);
  output_classes.Resize(height,1);
  reliabilities.Resize(height,1);

  for(int y=0; y<height; y++)
  {
//      result[y].resize(width);
      for(int x=0; x<4;x++)
       output_rects(y,x)=data[y*width+x];
      reliabilities(y,0)=data[y*width+4];
      output_classes(y,0)=data[y*width+5];
  }
/*
  DebugImage->SetColorModel(ubmRGB24,false);
  InputImage->ConvertTo(*DebugImage);
  Graph.SetCanvas(DebugImage);

  UAFont *class_font=GetFont("Tahoma",14);

  for(int i=0; i<result.size(); i++)
  {
      int xmin, ymin, xmax, ymax;
      ymin = (int)(result[i][0]);
      xmin = (int)(result[i][1]);
      ymax = (int)(result[i][2]);
      xmax = (int)(result[i][3]);

      Graph.SetPenColor(0x00FF00);
      Graph.Rect(xmin, ymin, xmax, ymax);

      double conf = result[i][4];
      int cls = static_cast<int>(result[i][5]);

      std::stringstream ss;

      if(ClassedList.size()>cls)
      {
          ss<<ClassedList[cls].c_str()<<"["<<conf<<"]";
      }
      else
      {
          ss<<"Unclassified";
      }

      if(class_font)
      {
        Graph.SetFont(class_font);
        Graph.Text(ss.str(),xmin, ymax+3);
      }
  }*/
 }
 catch (py::error_already_set const &)
 {
  std::string perrorStr = parse_python_exception();
  LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Python error: ")+perrorStr);
 }

 return true;
}
// --------------------------

}
#endif



