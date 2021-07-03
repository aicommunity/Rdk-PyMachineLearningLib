#ifndef RDK_TPyUBitmapClassifierH
#define RDK_TPyUBitmapClassifierH

#include "TPyComponent.h"
#include "../../Rdk-CRLib/Core/UClassifierBase.h"

namespace RDK {

class TPyUBitmapClassifier: public UClassifierBase, public TPyComponent
{
public: // Свойства
/// Целое число, определяющее цветовую модель, на которую рассчитана сеть
/// ubmRGB24=3 - цветное изображение
/// umbY8=400 - черно-белое изображение
ULProperty<int,TPyUBitmapClassifier, ptPubParameter> ImageColorModel;

/// Путь (прямой или относительный) к файлу с весами
ULProperty<std::string,TPyUBitmapClassifier, ptPubParameter> WeightsPath;

/// Включает передачу пути к конфигурации в скрипт
/// если используется - в скрипте д.б. реализован метод initialize_weights(self, weights_path)
/// метод в Питоне должен вернуть что-либо (напр. True) в знак штатного завершения работы
ULProperty<bool,TPyUBitmapClassifier, ptPubParameter> UseWeightsPath;

/// Включает использование пути к весам от папки проекта
ULProperty<bool,TPyUBitmapClassifier, ptPubParameter> UseRelativeWeightsPath;

protected: // Временные переменные

cv::Mat ProcessedMat;

UGraphics Graph;
UBitmap Canvas;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyUBitmapClassifier(void);
virtual ~TPyUBitmapClassifier(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TPyUBitmapClassifier* New(void);
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
protected:
bool APythonInitialize(void);

// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool APyDefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool APyBuild(void);

// Сброс процесса счета без потери настроек
virtual bool APyReset(void);


/// Обрабатывает одно изображение
virtual bool ClassifyBitmap(UBitmap &bmp, MDVector<double> &output_confidences, double conf_thresh, int &class_id, bool &is_classified);
// --------------------------
};


}

#endif



