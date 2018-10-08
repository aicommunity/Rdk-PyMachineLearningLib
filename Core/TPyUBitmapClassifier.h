#ifndef RDK_TPyUBitmapClassifierH
#define RDK_TPyUBitmapClassifierH

#include "TPythonIntegrationInclude.h"
#include "../../../Rdk/Deploy/Include/rdk.h"


namespace RDK {

class TPyUBitmapClassifier: public RDK::UNet
{
public: // Свойства
/// Входное изображение
/// UPropertyInputData<UBitmap,TPyUBitmapClassifier> InputImage;

/// Входное изображение
ULProperty<std::string,TPyUBitmapClassifier, ptPubParameter> PythonScriptFileName;

//Входные матрицы с данными об обнаружениях
/// Содержит изображения (обработанные) для классификации
UPropertyInputData<std::vector<UBitmap>, TPyUBitmapClassifier, ptPubInput> InputImages;

/// Выходная матрица с классами объектов
UPropertyOutputData<std::vector<int>,TPyUBitmapClassifier, ptPubOutput> OutputClasses;

boost::python::object IntegrationInterface;
boost::python::object IntegrationInterfaceInstance;

protected: // Переменные состояния

UGraphics Graph;
UBitmap Canvas;
bool Initialized;

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



