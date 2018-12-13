#ifndef RDK_TDarknetUBitmapClassifierCPP
#define RDK_TDarknetUBitmapClassifierCPP

#include "TDarknetUBitmapClassifier.h"
#include <iostream>

namespace RDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------  //DetectionClass("DetectionClass",this),
TDarknetUBitmapClassifier::TDarknetUBitmapClassifier(void)
: InputImages("InputImages",this),
  OutputClasses("OutputClasses",this),
  ImageColorModel("ImageColorModel",this),
  NumClasses("NumClasses",this),
  ProbabilityThreshold("ProbabilityThreshold",this),
  ObjectnessThreshold("ObjectnessThreshold",this),
  ConfigPath("ConfigPath",this),
  WeightsPath("WeightsPath",this),
  OutputConfidences("OutputConfidences", this)
{
}

TDarknetUBitmapClassifier::~TDarknetUBitmapClassifier(void)
{
}
// --------------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
TDarknetUBitmapClassifier* TDarknetUBitmapClassifier::New(void)
{
 return new TDarknetUBitmapClassifier;
}
// --------------------------


void TDarknetUBitmapClassifier::AInit(void)
{
    if(!Initialized)
    {
       if(!Initialize())
           return;
    }
}

void TDarknetUBitmapClassifier::AUnInit(void)
{
}
// --------------------------
// ������� ������ ���������� ������
// --------------------------
bool TDarknetUBitmapClassifier::Initialize(void)
{
    if(ConfigPath->size()==0 || WeightsPath->size()==0)
    {
        LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("TDarknetUBitmapClassifier error: ConfigPath or WeightsPath is empty!"));
        return false;
    }
    try {
        Network = load_network(const_cast<char*>(ConfigPath->c_str()), const_cast<char*>(WeightsPath->c_str()), 0);
        if(Network)
            TopLayer = &(Network->layers[Network->n-1]);
        /*set_batch_network(Network, 1);*/
    } catch (...) {
        LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("TDarknetUBitmapClassifier error: Unhandled exception"));
        return false;
    }

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


// �������������� �������� �� ��������� � ����� �������� �����
bool TDarknetUBitmapClassifier::ADefault(void)
{
 NumClasses=2;
 return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TDarknetUBitmapClassifier::ABuild(void)
{
 return true;
}

// ����� �������� ����� ��� ������ ��������
bool TDarknetUBitmapClassifier::AReset(void)
{
 return true;
}

// ��������� ������ ����� �������
bool TDarknetUBitmapClassifier::ACalculate(void)
{
 if(!InputImages.IsConnected())
  return true;

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
         ////////////////////////////////////////////////////////////
         /// ����� ����� ��� ��������� ����������� �����

         image img = UBitmapToImage(b);

         if(img.data==NULL)
         {
             LogMessageEx(RDK_EX_WARNING, __FUNCTION__, std::string("InputImage not converted correctly to darknet image"));
             (*OutputClasses)[i] = -3;
             for(int nc=0; nc<(*OutputConfidences).GetCols(); nc++)
                (*OutputConfidences)(i,nc)=0.0f;
             continue;
         }

         int nboxes = 0;
         image sized = resize_image(img, Network->w, Network->h);
         float *X = sized.data;
         clock_t time=clock();
         network_predict(Network, X);
         detection *dets = get_network_boxes(Network, 1, 1, ProbabilityThreshold, 0, 0, 0, &nboxes);
         printf("Predicted in %f seconds.\n", sec(clock()-time));

         int t = TopLayer->side*TopLayer->side*TopLayer->n;
         do_nms_obj(dets, nboxes, TopLayer->classes, 0.1);

         std::vector<std::vector<double> > result;

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


             /*if(FilterClassesList)
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
             }*/


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

         (*OutputClasses)[i] = 1;
         (*OutputConfidences)(i,0)=0.0f;
         (*OutputConfidences)(i,1)=1.0f;

         /*try
         {
          py::object retval = IntegrationInterfaceInstance.attr("classify")(m);

          //std::vector<float> res = boost::python::extract<std::vector<float> >(retval);
          np::ndarray ndarr = boost::python::extract< np::ndarray  >(retval);
          int dms = ndarr.get_nd();

          if(dms>2)
          {
              LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TDarknetUBitmapClassifier error: Returned array with incorrect dimensions"));
              return true;
          }

          const Py_intptr_t* shp = ndarr.get_shape();
          long height = shp[0];
          long width  = shp[1];

          const Py_intptr_t *strides = ndarr.get_strides();
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
              LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TDarknetUBitmapClassifier error: NumClasses not equals to returned confidences count"));
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
          LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TDarknetUBitmapClassifier error: ")+perrorStr);
         } 
         catch(...)
         {
             LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("Unknown exception"));
         }*/
     }
 }

 return true;
}
// --------------------------

image TDarknetUBitmapClassifier::UBitmapToImage(const UBitmap& ub)
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

}
#endif



