#ifndef RDK_TPyObjectDetectorH
#define RDK_TPyObjectDetectorH

#include "TPythonIntegrationInclude.h"
#include "TPythonIntegrationUtil.h"

namespace RDK {

class TPyObjectDetector: public RDK::UNet
{
public: // Свойства
/// Входное изображение
UPropertyInputData<UBitmap, TPyObjectDetector, ptPubInput> InputImage;

/// Целое число, определяющее цветовую модель, на которую рассчитана сеть
/// ubmRGB24=3 - цветное изображение
/// umbY8=400 - черно-белое изображение
ULProperty<int,TPyObjectDetector, ptPubParameter> ImageColorModel;

/// Количество классов объектов (какой размер будет у вектора
//ULProperty<int,TPyObjectDetector, ptPubParameter> NumClasses;


/// Выходная матрица с классами объектов
//UPropertyOutputData<std::vector<int>,TPyObjectDetector, ptPubOutput> OutputClasses;

/// Выходная матрица. Количество столбцов по числу объектов, количество строк в столбце по числу классов
/// Формат матрицы:
/// Высота по количеству объектов
/// Ширина 4+1=Left; Top; Right; Bottom; ClassNumber
UPropertyOutputData<MDMatrix<double>, TPyObjectDetector> OutputObjects;

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
TPyObjectDetector(void);
virtual ~TPyObjectDetector(void);
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
virtual TPyObjectDetector* New(void);
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



