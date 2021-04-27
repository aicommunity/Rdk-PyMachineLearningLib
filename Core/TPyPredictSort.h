#ifndef RDK_TPyPredictSortH
#define RDK_TPyPredictSortH

#include "TPyComponent.h"


namespace RDK {

class TPyPredictSort : public TPyComponent
{
public: // Свойства  

/// Директория, куда сохранится результат предсказания
ULProperty<std::string, TPyPredictSort> WorkingDir;

/// Директория с изображения для предсказания
ULProperty<std::string, TPyPredictSort> ImagesDir;

/// Путь к весам нейронной сети
ULProperty<std::string, TPyPredictSort> WeightPath;

/// Путь к конфигурации нейронной сети
ULProperty<std::string, TPyPredictSort> ConfigPath;

/// Нужно ли сортировать изображения по классам по папкам
ULProperty<bool, TPyPredictSort> SortImages;

/// Флаг остановки предсказания
ULProperty<bool, TPyPredictSort> StopNow;

/// Текущее состояния потока предсказания
// true  - поток исполняется
// false - поток не исполянется
ULProperty<bool, TPyPredictSort, ptPubState> ThreadIsAlive;

// Управляющие обучением параметры (и информационные)
/// Флаг включения обучения
// обучение начнется при StartTraining=true и PredictStatus=0
ULProperty<bool, TPyPredictSort> StartPredict;

/// Флаг завершения предсказания
ULProperty<bool, TPyPredictSort, ptPubState> PredictionEnded;

/// Статус предсказания
// -1 - ошибка (исключение внутри потока питона) либо успешное преждевременное завершение (stop_training, stop_now)).
// После обработки в компоненте сбрасывается в 0
// 0 - ничего не происходит (можно запускать предсказание)
// 1 - идет предсказание
// 2 - предсказание успешно завершено
ULProperty<int, TPyPredictSort, ptPubState> PredictStatus;

protected: // Временные переменные


public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyPredictSort(void);
virtual ~TPyPredictSort(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TPyPredictSort* New(void);
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

// Выполняет расчет этого объекта
virtual bool ACalculate(void);


// Проверяет входные параметры перед запуском python-функции
virtual bool CheckInputParameters();
// --------------------------
};


}

#endif



