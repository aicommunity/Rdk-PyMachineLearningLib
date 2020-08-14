#ifndef RDK_TPyClassifierTrainerH
#define RDK_TPyClassifierTrainerH

#include "TPyComponent.h"




namespace RDK {

class TPyClassifierTrainer: public TPyComponent
{
public: // Свойства  
/// Папка с изображениями для обучения
/*
может представлять собой исходную папку данных в формате
    main_directory/
    ...class_a/
    ......a_image_1.jpg
    ......a_image_2.jpg
    ...class_b/
    ......b_image_1.jpg
    ......b_image_2.jpg
*/
/// Или три папки с аналогичной структурой train, val, test
/// Или если вход - список, то три файла .txt файла со строчками "путь_до_картинки класс",
/// содержащие разбиение для выборок train, val, tes
ULProperty<std::string, TPyClassifierTrainer> TrainDataDir;

/// Директория, куда сохранится перераспредленный датасет, также веса метрики, графики, матрица ошибок
ULProperty<std::string, TPyClassifierTrainer> WorkingDir;

/// Имя архитектуры
/// Варианты: SqueezeNet, MobileNet, MobileNetV2, InceptionV3, VGG16, ResNet50,
/// ResNet101, DenseNet121, DenseNet169, NASNetMobile, NASNetLarge
ULProperty<std::string, TPyClassifierTrainer> ArchitectureName;

/// Имя набора данных для сохранения в файле конфига и названии весов
ULProperty<std::string, TPyClassifierTrainer> DatasetName;

/// [percent_train, percent_val, percent_test] - параметры разбиения выборки,
/// если используется основная директория (без разбиения на train val test, по умолчанию 70, 20, 10)
ULProperty<std::vector<int>, TPyClassifierTrainer> SplitRatio;

/// Сохранять ли текстовые файлы разбиений.
ULProperty<bool, TPyClassifierTrainer> SaveSplits;

/// Копировать ли в рабочую директорию картинки разбитые на train val test
/// согласно сгенерированным разбиениям из текстовых файлов
ULProperty<bool, TPyClassifierTrainer> CopySplittedImages;

/// Стоит ли приравнивать валидационную выборку тестовой.
/// В случае если разбиение равно 70, 20, 10 и TestEqualVal=True, то 10% выборки не будет использовано.
ULProperty<bool, TPyClassifierTrainer> TestEqualVal;

/// Параметры изображения (width, height, channels)
ULProperty<std::vector<int>, TPyClassifierTrainer> ImageSize;

/// Кол-во эпох для обучения
ULProperty<int, TPyClassifierTrainer> Epochs;

/// Learning Rate
ULProperty<float, TPyClassifierTrainer> LearningRate;

/// Размеры трех batch-ей, для train, val и test
ULProperty<std::vector<int>, TPyClassifierTrainer> BatchSizes;

/// Начальные веса. По умолчанию "imagenet" иначе random или путь до конкретных весов
ULProperty<std::string, TPyClassifierTrainer> Weights;

/// Количество тренируемых слоев. N последних
ULProperty<int, TPyClassifierTrainer> LayersToBeTrained;

/// Классы, опционально(список), если учимся не на всех классах датасета
ULProperty<std::vector<std::string>, TPyClassifierTrainer> Classes;

/// Число, равное параметру patience
/// (количество эпох, которое может быть проведено без улучшения значения функции потерь, до остановки обучения)
ULProperty<int, TPyClassifierTrainer> EarlyStop;

/// Интервал сохранения весов (в случае варианта для tf 1.* - n эпох) (в случае варианта tf 2.* - n батчей)
ULProperty<int, TPyClassifierTrainer> SavingInterval;

/// Cохранять ли только лучшую модель по параметру val_loss
ULProperty<bool, TPyClassifierTrainer> SaveBestOnly;

/// Флаг включения обучения
// обучение начнется при StartTraining=true и TrainingInProgress=false
// при запуске обучения данный флаг становится false
ULProperty<bool, TPyClassifierTrainer> StartTraining;

/// Флаг остановки обучения
// останавливает обучение и делает тест, если прошло больше одной эпохи, потом завершается поток
ULProperty<bool, TPyClassifierTrainer> StopTraining;

/// Флаг остановки обучения (закончит без тестов)
ULProperty<bool, TPyClassifierTrainer> StopNow;


/// Статус обучения
// 0 - ничего не происходит
// 1 - обучение
// 2 - тестирование после обучения
// 3 -
ULProperty<int, TPyClassifierTrainer, ptPubState> TrainingStatus;

/// Текущая эпоха
ULProperty<int, TPyClassifierTrainer, ptPubState> Epoch;

/// Текущая ошибка на тренировочном наборе
ULProperty<float, TPyClassifierTrainer, ptPubState> TrainLoss;

/// Текущая точность на тренировочном наборе
ULProperty<float, TPyClassifierTrainer, ptPubState> TrainAcc;

/// Текущая ошибка на валидационном наборе
ULProperty<float, TPyClassifierTrainer, ptPubState> ValLoss;

/// Текущая точность на валидационном наборе
ULProperty<float, TPyClassifierTrainer, ptPubState> ValAcc;

/// Текущий прогресс
ULProperty<float, TPyClassifierTrainer, ptPubState> Progress;



protected: // Временные переменные



public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyClassifierTrainer(void);
virtual ~TPyClassifierTrainer(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TPyClassifierTrainer* New(void);
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
virtual bool APyCalculate(void);


// Проверяет входные параметры перед запуском python-функции
bool CheckInputParameters();
// --------------------------
};


}

#endif



