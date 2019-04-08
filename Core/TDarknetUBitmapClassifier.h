#ifndef RDK_TDarknetUBitmapClassifierH
#define RDK_TDarknetUBitmapClassifierH

#include "TPythonIntegrationInclude.h"
//#include "TPythonIntegrationUtil.h"
#include "../ThirdParty/darknet/include/darknet_utils.h"

namespace RDK {

class TDarknetUBitmapClassifier: public RDK::UNet
{
public: // Свойства
/// Входное изображение
/// UPropertyInputData<UBitmap,TDarknetUBitmapClassifier> InputImage;

//Входные матрицы с данными об обнаружениях
/// Содержит изображения (обработанные) для классификации
UPropertyInputData<std::vector<UBitmap>, TDarknetUBitmapClassifier, ptPubInput> InputImages;

/// Целое число, определяющее цветовую модель, на которую рассчитана сеть
/// ubmRGB24=3 - цветное изображение
/// umbY8=400 - черно-белое изображение
ULProperty<int,TDarknetUBitmapClassifier, ptPubParameter> ImageColorModel;

/// Количество классов объектов (какой размер будет у вектора
ULProperty<int,TDarknetUBitmapClassifier, ptPubParameter> NumClasses;
/// Порог уверенности, выше которого обнаружения считаются валидными (probability)
ULProperty<float,TDarknetUBitmapClassifier, ptPubParameter> ProbabilityThreshold;
/// Порог объектности, выше которого обнаружения считаются валидными (objectness)
ULProperty<float,TDarknetUBitmapClassifier, ptPubParameter> ObjectnessThreshold;

/// Выходная матрица с классами объектов
UPropertyOutputData<std::vector<int>,TDarknetUBitmapClassifier, ptPubOutput> OutputClasses;

/// Выходная матрица. Количество столбцов по числу объектов, количество строк в столбце по числу классов
/// Каждое значение - уверенность класса
UPropertyOutputData<MDMatrix<double>, TDarknetUBitmapClassifier> OutputConfidences;

///Путь к файлу конфигурации Йолы
ULProperty<std::string,TDarknetUBitmapClassifier, ptPubParameter> ConfigPath;
///Путь к файлу весов Йолы
ULProperty<std::string,TDarknetUBitmapClassifier, ptPubParameter> WeightsPath;

protected: // Переменные состояния

UGraphics Graph;
UBitmap Canvas;

bool Initialized;

network *Network;
//layer* TopLayer;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TDarknetUBitmapClassifier(void);
virtual ~TDarknetUBitmapClassifier(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TDarknetUBitmapClassifier* New(void);
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
protected:
bool Initialize(void);

virtual void AInit(void);
virtual void AUnInit(void);

// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool ABuild(void);

// Сброс процесса счета без потери настроек
virtual bool AReset(void);

// Выполняет расчет этого объекта
virtual bool ACalculate(void);
// --------------------------
image UBitmapToImage(const UBitmap& ub);
};


}

#endif



