#define NO_IMPORT_ARRAY

#ifndef RDK_TDarknetObjectDetectorCPP
#define RDK_TDarknetObjectDetectorCPP

#include "TDarknetObjectDetector.h"
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

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------  //DetectionClass("DetectionClass",this),
TDarknetObjectDetector::TDarknetObjectDetector(void)
: InputImage("InputImage",this),
  //Initialized(false),
  OutputObjects("OutputObjects",this),
  ImageColorModel("ImageColorModel",this),
  //ModelPathYOLO("ModelPathYOLO",this),
  //AnchorsPathYOLO("AnchorsPathYOLO",this),
  //ClassesPathYOLO("ClassesPathYOLO",this),
  ProbabilityThreshold("ProbabilityThreshold",this),
  ObjectnessThreshold("ObjectnessThreshold",this),
  OutputImage("OutputImage",this),
  FilterClassesList("FilterClassesList",this),
  ConfigPath("ConfigPath",this),
  WeightsPath("WeightsPath",this),
  //LoadTargetClassesYOLO("LoadTargetClassesYOLO",this)
  //OutputConfidences("OutputConfidences", this)
  ClassesList("ClassesList",this),
  NumClasses("NumClasses",this)
{
    /*AddLookupProperty("PythonScriptPath",ptPubParameter, new UVProperty<std::string,TDarknetObjectDetector>(this,
                 &TDarknetObjectDetector::SetPythonClassifierScriptPath,&TDarknetObjectDetector::GetPythonClassifierScriptPath));*/

    /*AddLookupProperty("NumTargetClassesYOLO",ptPubParameter, new UVProperty<int,TDarknetObjectDetector>(this,
                 &TDarknetObjectDetector::SetNumTargetClassesYOLO,&TDarknetObjectDetector::GetNumTargetClassesYOLO));

    AddLookupProperty("NumChangeClassesYOLO",ptPubParameter, new UVProperty<int,TDarknetObjectDetector>(this,
                 &TDarknetObjectDetector::SetNumChangeClassesYOLO,&TDarknetObjectDetector::GetNumChangeClassesYOLO));
*/
}

/*bool TDarknetObjectDetector::SetPythonClassifierScriptPath(const std::string& path)
{
    PythonScriptFileName = path;
    Initialized=false;
    return true;
}
const std::string & TDarknetObjectDetector::GetPythonClassifierScriptPath(void) const
{
    return PythonScriptFileName;
}
*/
/*
bool TDarknetObjectDetector::SetNumTargetClassesYOLO(const int& num)
{
    NumTargetClassesYOLO = num;
    TargetClassesYOLO->resize(num);
    return true;
}
const int& TDarknetObjectDetector::GetNumTargetClassesYOLO(void) const
{
    return NumTargetClassesYOLO;
}

bool TDarknetObjectDetector::SetNumChangeClassesYOLO(const int& num)
{
    NumChangeClassesYOLO = num;
    ChangeClassesYOLO->resize(num);
    return true;
}
const int& TDarknetObjectDetector::GetNumChangeClassesYOLO(void) const
{
    return NumChangeClassesYOLO;
}
*/

TDarknetObjectDetector::~TDarknetObjectDetector(void)
{
    //В примере очистки нет, предполагаю, что это ссылка на полноценную
    //структуру данных где-то внутри, которая уничтожается в другом месте
    /*if(Network)
        delete Network;*/
}
// --------------------------


// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
TDarknetObjectDetector* TDarknetObjectDetector::New(void)
{
 return new TDarknetObjectDetector;
}
// --------------------------

void TDarknetObjectDetector::AInit(void)
{
    if(!Initialized)
    {
       if(!Initialize())
           return;
    }
}

// --------------------------
// Скрытые методы управления счетом
// --------------------------
bool TDarknetObjectDetector::Initialize(void)
{
    Network = load_network(const_cast<char*>(ConfigPath->c_str()), const_cast<char*>(WeightsPath->c_str()), 0);
    TopLayer = Network->layers[Network->n-1];
    set_batch_network(Network, 1);
    if(Network)
    {
        Initialized=true;
        return true;
    }
    else
    {
        Initialized=false;
        return false;
    }

}

void TDarknetObjectDetector::AUnInit(void)
{
}

// Восстановление настроек по умолчанию и сброс процесса счета
bool TDarknetObjectDetector::ADefault(void)
{
 Initialized=false;
 ProbabilityThreshold = 0.0f;
 ObjectnessThreshold = 0.0f;
 FilterClassesList=false;
 return true;
}
// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool TDarknetObjectDetector::ABuild(void)
{
 return true;
}

// Сброс процесса счета без потери настроек
bool TDarknetObjectDetector::AReset(void)
{
 //Initialized = false;
 return true;
}

// Выполняет расчет этого объекта
bool TDarknetObjectDetector::ACalculate(void)
{
// if(!Initialized)
// {
//    if(!Initialize())
//        return true;
// }

 if(!InputImage.IsConnected())
  return true;

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

 /// Тут считаем
 std::vector<std::vector<double> > result;

 ////////////////////////////////////////////////////////////
 /// Здесь место для обработки изображения сетью

 image img = UBitmapToImage(b);
 if(img.data==NULL)
 {
     LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("InputImage not converted correctly to darknet image"));
     return true;
 }

 int nboxes = 0;
 image sized = resize_image(img, Network->w, Network->h);
 float *X = sized.data;
 clock_t time=clock();
 network_predict(Network, X);
 detection *dets = get_network_boxes(Network, 1, 1, ProbabilityThreshold, 0, 0, 0, &nboxes);
 printf("Predicted in %f seconds.\n", sec(clock()-time));

 int t = TopLayer.side*TopLayer.side*TopLayer.n;
 do_nms_obj(dets, nboxes, TopLayer.classes, 0.1);

 result.clear();

 for(int i=0; i<nboxes; i++)
 {
     detection &d = dets[i];
     if(d.objectness<ObjectnessThreshold)
         continue;

     int cmax = -1;
     float probmax=-1.0;
     for(int ci=0; ci<d.classes; ci++)
     {
         if(d.prob[ci]>probmax)
         {
             probmax = d.prob[ci];
             cmax = ci;
         }
     }

     //printf("Objectness is %f.\n", d.objectness);


     if(FilterClassesList)
     {
        if(ClassesList->size()>0)
        {
            bool skip=true;
            for(int ci=0; ci<ClassesList->size(); ci++)
            {
                if((*ClassesList)[ci]==cmax)
                {
                    skip=false;
                }
            }
            if(skip)
                continue;
        }
     }


     double xmin = d.bbox.x-d.bbox.w/2;
     double xmax = d.bbox.x+d.bbox.w/2;
     double ymin = d.bbox.y-d.bbox.h/2;
     double ymax = d.bbox.y+d.bbox.h/2;
     std::vector<double> res;
     res.resize(6);
     res[0] = xmin*w;
     res[1] = ymin*h;
     res[2] = xmax*w;
     res[3] = ymax*h;

     res[4] = probmax;
     res[5] = cmax;
     result.push_back(res);
     //std::cerr<<d.bbox.x<<" "<<d.bbox.y<<" "<<d.bbox.w<<" "<<d.bbox.h<<" "<<probmax<<" "<<d.objectness<<" "<<voc_names[cmax]<<"\n";
 }

 free_detections(dets, nboxes);
 free_image(img);
 free_image(sized);

 OutputObjects->Resize(result.size(), 6);

 for(int i=0; i<result.size(); i++)
 {
     (*OutputObjects)(i, 0) = result[i][0];
     (*OutputObjects)(i, 1) = result[i][1];
     (*OutputObjects)(i, 2) = result[i][2];
     (*OutputObjects)(i, 3) = result[i][3];
     (*OutputObjects)(i, 4) = result[i][4];
     (*OutputObjects)(i, 5) = result[i][5];
 }

 ////////////////////////////////////////////////////////////
  /*UAFont *class_font=GetFont("Tahoma",14);

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

//      if(ClassedList.size()>cls)
//      {
//          ss<<"; C="<<ClassedList[cls].c_str();
//      }
//      else
//      {
//          ss<<"; C="<<cls;
//      }
      ss<<"]";

      if(class_font)
      {
        Graph.SetFont(class_font);
        Graph.Text(ss.str(),xmin, ymax+3);
      }
  }
  */
 return true;
}
// --------------------------

image TDarknetObjectDetector::UBitmapToImage(const UBitmap& ub)
{
    int h = ub.GetHeight();
    int w = ub.GetWidth();
    int cm = ub.GetColorModel();

    int step = ub.GetLineByteLength();
    int c = ub.GetPixelByteLength();

    if(cm==ubmRGB24)
    {
        image im = make_image(w, h, c);
        unsigned char *data = (unsigned char *)ub.GetData();
        for(int i = 0; i < h; ++i){
            for(int k= 0; k < c; ++k){
                for(int j = 0; j < w; ++j){
                    im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
                }
            }
        }
        return im;

    }
    else if(cm==ubmY8)
    {
        return image();
    }
    else
    {
        return image();
    }
}
/*
 * Просто очень простой пример
image ipl_to_image(IplImage* src)
{
    int h = src->height;
    int w = src->width;
    int c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)src->imageData;
    int step = src->widthStep;
    int i, j, k;

    for(i = 0; i < h; ++i){
        for(k= 0; k < c; ++k){
            for(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    return im;
}*/

}
#endif



