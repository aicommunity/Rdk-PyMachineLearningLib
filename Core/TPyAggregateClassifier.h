#ifndef RDK_TPyAggregateClassifierH
#define RDK_TPyAggregateClassifierH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>

namespace RDK {

class TPyAggregateClassifier: public RDK::UNet
{
public: // Свойства
/// Параметр
ULProperty<int,TPyAggregateClassifier> Param1;

/// Входное изображение
UPropertyInputData<UBitmap,TPyAggregateClassifier> InputImage;

/// Входное изображение
ULProperty<std::string,TPyAggregateClassifier, ptPubParameter> InputFile;
/// Выходная матрица с рамками объектов
UPropertyOutputData<MDMatrix<int>,TPyAggregateClassifier> Detections;

/// Выходная матрица с рамками объектов
UPropertyOutputData<MDMatrix<int>,TPyAggregateClassifier> DetectionClass;

/// Выходная матрица с рамками объектов
UPropertyOutputData<MDMatrix<double>,TPyAggregateClassifier> DetectionReliability;

UPropertyOutputData<UBitmap,TPyAggregateClassifier> DebugImage;

boost::python::object IntegrationInterface;
boost::python::object IntegrationInterfaceInstance;

protected: // Переменные состояния

UGraphics Graph;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyAggregateClassifier(void);
virtual ~TPyAggregateClassifier(void);
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
virtual TPyAggregateClassifier* New(void);
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



