#ifndef RDK_TPyBaseTrainerH
#define RDK_TPyBaseTrainerH

#include "TPyComponent.h"


namespace RDK {

/// Абстрактный базовый класс компонентов обучения нейронных сетей разных типов
class TPyBaseTrainer: public TPyComponent
{
public: // Свойства

/// Папка с изображениями для обучения (может быть несколько путей)
ULProperty<std::vector<std::string>, TPyBaseTrainer> TrainDataDir;

/// Директория, куда сохранится перераспредленный датасет, также веса метрики, графики, матрица ошибок и проч.
ULProperty<std::string, TPyBaseTrainer> WorkingDir;

/// Имя архитектуры нейронной сети
ULProperty<std::string, TPyBaseTrainer> ArchitectureName;

/// Параметры разбиения выборки [percent_train, percent_val, percent_test]
/// если используется основная директория (без разбиения на train val test, по умолчанию [0.7, 0.2, 0.1])
ULProperty<std::vector<int>, TPyBaseTrainer> SplitRatio;

/// Сохранять ли текстовые файлы разбиений
ULProperty<bool, TPyBaseTrainer> SaveSplits;

/// Кол-во эпох для обучения
ULProperty<int, TPyBaseTrainer> Epochs;

/// Начальные веса (путь либо название)
ULProperty<std::string, TPyBaseTrainer> Weights;

/// Число, равное параметру patience
/// (количество эпох, которое может быть проведено без улучшения значения функции потерь, до остановки обучения) 
ULProperty<int, TPyBaseTrainer> EarlyStop;

/// Интервал сохранения весов (в случае варианта для tf 1.* - n эпох) (в случае варианта tf 2.* - n батчей)
ULProperty<int, TPyBaseTrainer> SavingInterval;

/// Cохранять ли только лучшую модель по параметру val_loss
ULProperty<bool, TPyBaseTrainer> SaveBestOnly;

/// Флаг включения обучения
// обучение начнется при StartTraining=true и TrainingInProgress=0
ULProperty<bool, TPyBaseTrainer> StartTraining;

/// Флаг остановки обучения
// останавливает обучение и делает тест, если прошло больше одной эпохи, потом функция завершается
ULProperty<bool, TPyBaseTrainer> StopTraining;

/// Флаг остановки обучения (закончит без тестов)
ULProperty<bool, TPyBaseTrainer> StopNow;

/// Статус обучения
// -1 - ошибка (исключение внутри потока питона). После обработки в компоненте сбрасывается в 0
// 0 - ничего не происходит (можно запускать обучение)
// 1 - обучение
// 2 - тестирование после обучения
// 3 - успешное завершение работы. После обработки статус сбрасывается в 0
//     (законечно обучение, либо успешное преждевременное завершение (stop_training, stop_now))
ULProperty<int, TPyBaseTrainer, ptPubState> TrainingStatus;

/// Текущая эпоха
ULProperty<int, TPyBaseTrainer, ptPubState> Epoch;

/// Текущий прогресс
ULProperty<float, TPyBaseTrainer, ptPubState> Progress;


protected: // Временные переменные



public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyBaseTrainer(void);
virtual ~TPyBaseTrainer(void);
// --------------------------

// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
protected:
virtual bool APythonInitialize(void);

// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool APyDefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool APyBuild(void);

// Сброс процесса счета без потери настроек
virtual bool APyReset(void) final;

// Выполняет расчет этого объекта
virtual bool ACalculate(void);


// Проверяет входные параметры перед запуском python-функции
virtual bool CheckInputParameters() = 0;
// --------------------------
};


}

#endif



