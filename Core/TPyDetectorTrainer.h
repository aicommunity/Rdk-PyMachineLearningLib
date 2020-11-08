#ifndef RDK_TPyDetectorTrainerH
#define RDK_TPyDetectorTrainerH

#include "TPyComponent.h"




namespace RDK {

class TPyDetectorTrainer: public TPyComponent
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
ULProperty<std::string, TPyDetectorTrainer> TrainDataDir;

/// Директория, куда сохранится перераспредленный датасет, также веса метрики, графики, матрица ошибок
ULProperty<std::string, TPyDetectorTrainer> WorkingDir;

/// Имя архитектуры
/// Варианты: SqueezeNet, MobileNet, MobileNetV2, InceptionV3, VGG16, ResNet50,
/// ResNet101, DenseNet121, DenseNet169, NASNetMobile, NASNetLarge
ULProperty<std::string, TPyDetectorTrainer> ArchitectureName;

/// Имя набора данных для сохранения в файле конфига и названии весов
ULProperty<std::string, TPyDetectorTrainer> DatasetName;

/// [percent_train, percent_val, percent_test] - параметры разбиения выборки,
/// если используется основная директория (без разбиения на train val test, по умолчанию 70, 20, 10)
ULProperty<std::vector<int>, TPyDetectorTrainer> SplitRatio;

/// Сохранять ли текстовые файлы разбиений.
ULProperty<bool, TPyDetectorTrainer> SaveSplits;

/// Копировать ли в рабочую директорию картинки разбитые на train val test
/// согласно сгенерированным разбиениям из текстовых файлов
ULProperty<bool, TPyDetectorTrainer> CopySplittedImages;

/// Стоит ли приравнивать валидационную выборку тестовой.
/// В случае если разбиение равно 70, 20, 10 и TestEqualVal=True, то 10% выборки не будет использовано.
ULProperty<bool, TPyDetectorTrainer> TestEqualVal;

/// Параметры изображения (width, height, channels)
ULProperty<std::vector<int>, TPyDetectorTrainer> ImageSize;

/// Кол-во эпох для обучения
ULProperty<int, TPyDetectorTrainer> Epochs;

/// Learning Rate
ULProperty<float, TPyDetectorTrainer> LearningRate;

/// Размеры трех batch-ей, для train, val и test
ULProperty<std::vector<int>, TPyDetectorTrainer> BatchSizes;

/// Начальные веса. По умолчанию "imagenet" иначе random или путь до конкретных весов
ULProperty<std::string, TPyDetectorTrainer> Weights;

/// Количество тренируемых слоев. N последних
ULProperty<int, TPyDetectorTrainer> LayersToBeTrained;

/// Классы, опционально(список), если учимся не на всех классах датасета
ULProperty<std::vector<std::string>, TPyDetectorTrainer> Classes;

/// Число, равное параметру patience
/// (количество эпох, которое может быть проведено без улучшения значения функции потерь, до остановки обучения)
ULProperty<int, TPyDetectorTrainer> EarlyStop;

/// Интервал сохранения весов (в случае варианта для tf 1.* - n эпох) (в случае варианта tf 2.* - n батчей)
ULProperty<int, TPyDetectorTrainer> SavingInterval;

/// Cохранять ли только лучшую модель по параметру val_loss
ULProperty<bool, TPyDetectorTrainer> SaveBestOnly;

/// Флаг включения обучения
// обучение начнется при StartTraining=true и TrainingInProgress=false
// при запуске обучения данный флаг становится false
ULProperty<bool, TPyDetectorTrainer> StartTraining;

/// Флаг остановки обучения
// останавливает обучение и делает тест, если прошло больше одной эпохи, потом завершается поток
ULProperty<bool, TPyDetectorTrainer> StopTraining;

/// Флаг остановки обучения (закончит без тестов)
ULProperty<bool, TPyDetectorTrainer> StopNow;


/// Статус обучения
// 0 - ничего не происходит
// 1 - обучение
// 2 - тестирование после обучения
// 3 -
ULProperty<int, TPyDetectorTrainer, ptPubState> TrainingStatus;

/// Текущая эпоха
ULProperty<int, TPyDetectorTrainer, ptPubState> Epoch;

/// Текущая ошибка на тренировочном наборе
ULProperty<float, TPyDetectorTrainer, ptPubState> TrainLoss;

/// Текущая точность на тренировочном наборе
ULProperty<float, TPyDetectorTrainer, ptPubState> TrainAcc;

/// Текущая ошибка на валидационном наборе
ULProperty<float, TPyDetectorTrainer, ptPubState> ValLoss;

/// Текущая точность на валидационном наборе
ULProperty<float, TPyDetectorTrainer, ptPubState> ValAcc;

/// Текущий прогресс
ULProperty<float, TPyDetectorTrainer, ptPubState> Progress;



protected: // Временные переменные



public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyDetectorTrainer(void);
virtual ~TPyDetectorTrainer(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TPyDetectorTrainer* New(void);
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
bool CheckInputParameters();
// --------------------------
};


}

#endif



