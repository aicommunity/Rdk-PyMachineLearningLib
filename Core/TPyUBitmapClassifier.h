#ifndef PY_ARRAY_UNIQUE_SYMBOL
#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API
#endif
#ifndef RDK_TPyUBitmapClassifierH
#define RDK_TPyUBitmapClassifierH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "TPythonIntegrationInclude.h"


namespace RDK {

class TPyUBitmapClassifier: public RDK::UNet
{
public: // Свойства
/// Входное изображение
/// UPropertyInputData<UBitmap,TPyUBitmapClassifier> InputImage;

/// Входное изображение
//ULProperty<std::string,TPyUBitmapClassifier, ptPubParameter> PythonScriptFileName;

//Входные матрицы с данными об обнаружениях
/// Содержит изображения (обработанные) для классификации
UPropertyInputData<std::vector<UBitmap>, TPyUBitmapClassifier, ptPubInput> InputImages;

/// Целое число, определяющее цветовую модель, на которую рассчитана сеть
/// ubmRGB24=3 - цветное изображение
/// umbY8=400 - черно-белое изображение
ULProperty<int,TPyUBitmapClassifier, ptPubParameter> ImageColorModel;

/// Количество классов объектов (какой размер будет у вектора
ULProperty<int,TPyUBitmapClassifier, ptPubParameter> NumClasses;

/// Выходная матрица с классами объектов
UPropertyOutputData<std::vector<int>,TPyUBitmapClassifier, ptPubOutput> OutputClasses;

/// Выходная матрица. Количество столбцов по числу объектов, количество строк в столбце по числу классов
/// Каждое значение - уверенность класса
UPropertyOutputData<MDMatrix<double>, TPyUBitmapClassifier> OutputConfidences;

boost::python::object IntegrationInterface;
boost::python::object IntegrationInterfaceInstance;

protected: // Переменные состояния

UGraphics Graph;
UBitmap Canvas;
bool Initialized;
std::string PythonScriptFileName;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyUBitmapClassifier(void);
virtual ~TPyUBitmapClassifier(void);
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------

bool SetPythonClassifierScriptPath(const std::string& path);
const std::string &GetPythonClassifierScriptPath(void) const;

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

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
};


}

#endif



