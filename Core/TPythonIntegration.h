#ifndef RDK_TPythonIntegrationH
#define RDK_TPythonIntegrationH

#include "../../../Rdk/Deploy/Include/rdk.h"
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>

namespace RDK {

class TPythonIntegration: public RDK::UNet
{
public: // Свойства
/// Параметр
ULProperty<int,TPythonIntegration> Param1;

/// Входное изображение
UPropertyInputData<UBitmap,TPythonIntegration> InputImage;

/// Входное изображение
ULProperty<std::string,TPythonIntegration, ptPubParameter> InputFile;
/// Выходная матрица с рамками объектов
UPropertyOutputData<MDMatrix<int>,TPythonIntegration> Detections;

/// Выходная матрица с рамками объектов
UPropertyOutputData<MDMatrix<int>,TPythonIntegration> DetectionClass;

/// Выходная матрица с рамками объектов
UPropertyOutputData<MDMatrix<double>,TPythonIntegration> DetectionReliability;

UPropertyOutputData<UBitmap,TPythonIntegration> DebugImage;

boost::python::object IntegrationInterface;
boost::python::object IntegrationInterfaceInstance;

protected: // Переменные состояния

UGraphics Graph;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPythonIntegration(void);
virtual ~TPythonIntegration(void);
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
virtual TPythonIntegration* New(void);
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



