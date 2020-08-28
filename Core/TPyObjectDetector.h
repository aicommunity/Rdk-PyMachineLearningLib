#ifndef RDK_TPyObjectDetectorH
#define RDK_TPyObjectDetectorH

#include "TPyComponent.h"
#include "../../Rdk-CRLib/Core/UDetectorBase.h"
namespace RDK {

#define YOLOV2_INITTYPE 2
#define YOLOV3_INITTYPE 3

class TPyObjectDetector: public UDetectorBase, public TPyComponent
{
public: // Свойства
/// Целое число, определяющее цветовую модель, на которую рассчитана сеть
/// ubmRGB24=3 - цветное изображение
/// umbY8=400 - черно-белое изображение
ULProperty<int,TPyObjectDetector, ptPubParameter> ImageColorModel;

protected: // Временные переменные

UBitmap ProcessedBmp;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyObjectDetector(void);
virtual ~TPyObjectDetector(void);
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
protected:

// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool APyDefault(void);
virtual bool APyDefault2(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool APyBuild(void);
virtual bool APyBuild2(void);

// Сброс процесса счета без потери настроек
virtual bool APyReset(void);
virtual bool APyReset2(void);

// Выполняет расчет этого объекта
virtual bool APyCalculate(void);
virtual bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
// --------------------------
};


}

#endif



