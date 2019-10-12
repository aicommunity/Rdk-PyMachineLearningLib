#ifndef RDK_TPyObjectDetectorH
#define RDK_TPyObjectDetectorH

#include "TPyComponent.h"

namespace RDK {

#define YOLOV2_INITTYPE 2
#define YOLOV3_INITTYPE 3

class TPyObjectDetector: public TPyComponent
{
public: // Свойства
/// Входное изображение
UPropertyInputData<UBitmap, TPyObjectDetector> InputImage;

/// Выходное отладочное изображение
UPropertyOutputData<UBitmap, TPyObjectDetector> DebugImage;

/// Целое число, определяющее цветовую модель, на которую рассчитана сеть
/// ubmRGB24=3 - цветное изображение
/// umbY8=400 - черно-белое изображение
ULProperty<int,TPyObjectDetector, ptPubParameter> ImageColorModel;

///Отвечает за домножение на
ULProperty<bool,TPyObjectDetector, ptPubParameter> UseRelativeCoords;

///Отвечает за домножение на
ULProperty<bool,TPyObjectDetector, ptPubParameter> UseDebugImage;

/// Выходная матрица. Количество строк по числу объектов
/// Формат матрицы:
/// Высота по количеству объектов
/// Ширина 4+1=Left; Top; Right; Bottom; ClassNumber
UPropertyOutputData<MDMatrix<double>, TPyObjectDetector, ptOutput | ptPubState> OutputObjects;

/// Выходная матрица только прямоугольников с экранными координатами
UPropertyOutputData<MDMatrix<double>, TPyObjectDetector, ptOutput | ptPubState> OutputRects;

/// Выходная матрица идентификаторов классов
UPropertyOutputData<MDMatrix<int>, TPyObjectDetector, ptOutput | ptPubState> OutputClasses;

/// Выходная матрица оценок достоверностей
UPropertyOutputData<MDMatrix<double>, TPyObjectDetector, ptOutput | ptPubState> OutputReliability;

protected: // Временные переменные
UGraphics Graph;

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



