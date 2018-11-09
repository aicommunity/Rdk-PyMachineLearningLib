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
UPropertyInputData<UBitmap, TPyObjectDetector, ptPubOutput> OutputImage;

/// Целое число, определяющее цветовую модель, на которую рассчитана сеть
/// ubmRGB24=3 - цветное изображение
/// umbY8=400 - черно-белое изображение
ULProperty<int,TPyObjectDetector, ptPubParameter> ImageColorModel;

/// Количество классов объектов (какой размер будет у вектора
//ULProperty<int,TPyObjectDetector, ptPubParameter> NumClasses;model_path

///Тип инициализации: 1 - упрощенный, стандартный, через пути к модели, якорям, перечню классов, целевыми и заменами
///                   2 - инициализация через файл с конфигурацией и файл весов
ULProperty<int,TPyObjectDetector, ptPubParameter> InitializationTypeYOLO;

///Путь к файлу конфигурации Йолы
ULProperty<std::string,TPyObjectDetector, ptPubParameter> ConfigPathYOLO;
///Путь к файлу весов Йолы
ULProperty<std::string,TPyObjectDetector, ptPubParameter> WeightsPathYOLO;



///Путь к модели Йолы
ULProperty<std::string,TPyObjectDetector, ptPubParameter> ModelPathYOLO;
///Путь к файлу с якорями (оставить пустым если используем стандарт)
ULProperty<std::string,TPyObjectDetector, ptPubParameter> AnchorsPathYOLO;
///Путь к файлу с перечнем классов для используемой сети
ULProperty<std::string,TPyObjectDetector, ptPubParameter> ClassesPathYOLO;
///Список целевых классов
ULProperty<std::vector<std::string>,TPyObjectDetector, ptPubParameter> TargetClassesYOLO;
///Загружать список классов из соответствующего файла
ULProperty<bool,TPyObjectDetector, ptPubParameter> LoadTargetClassesYOLO;
///Список замен классов (изменить список классов в файле ClassesPathYOLO на свои имена)
///ВНИМАНИЕ!!! Число классов должно соответствовать числу классов в ClassesPathYOLO
ULProperty<std::vector<std::string>,TPyObjectDetector, ptPubParameter> ChangeClassesYOLO;

///Замена классов по значениям
///ULProperty<std::map<int, int>,TPyObjectDetector, ptPubParameter> ClassIndicesExchange;


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

int NumTargetClassesYOLO;
int NumChangeClassesYOLO;

std::vector<std::string> ClassedList;

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

bool SetNumTargetClassesYOLO(const int& num);
const int& GetNumTargetClassesYOLO(void) const;

bool SetNumChangeClassesYOLO(const int& num);
const int& GetNumChangeClassesYOLO(void) const;

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
bool Initialize(void);
//virtual void AInit(void);
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



