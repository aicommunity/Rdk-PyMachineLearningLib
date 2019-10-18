#ifndef RDK_TPySegmentatorH
#define RDK_TPySegmentatorH

#include "TPyComponent.h"

namespace RDK {

#define YOLOV2_INITTYPE 2
#define YOLOV3_INITTYPE 3

class TPySegmentator: public TPyComponent
{
public: // Свойства
/// Входное изображение
UPropertyInputData<UBitmap, TPySegmentator> InputImage;

/// Выходное изображение c результирующей маской
UPropertyOutputData<UBitmap, TPySegmentator> OutputImage;

/// Целое число, определяющее цветовую модель, на которую рассчитана сеть
/// ubmRGB24=3 - цветное изображение
/// umbY8=400 - черно-белое изображение
ULProperty<int,TPySegmentator, ptPubParameter> ImageColorModel;

/// Соответствие классов и цветов
ULProperty<std::vector<std::map<std::string, UColorT> >,TPySegmentator, ptPubParameter> ClassColors;

protected: // Временные переменные
UGraphics Graph;

UBitmap ProcessedBmp;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPySegmentator(void);
virtual ~TPySegmentator(void);
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
virtual bool Inference(UBitmap &bmp, UBitmap &mask);
// --------------------------
};


}

#endif



