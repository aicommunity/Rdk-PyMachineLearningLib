#ifndef RDK_TPySegmenterTrainerH
#define RDK_TPySegmenterTrainerH

#include "TPyBaseTrainer.h"




namespace RDK {

class TPySegmenterTrainer: public TPyBaseTrainer
{
public: // Свойства  

    // в TrainDataDir 2 пути:
    //      "basedir_ann" - путь к аннотациям базы
    //      "basedir_img" - путь к изображениям базы

/// Название обучающей базы
ULProperty<std::string, TPySegmenterTrainer> DatasetName;

/// Модель архитектуры (актуально только для unet)
ULProperty<std::string, TPySegmenterTrainer> Model;

/// размер, на котором будут обучаться изображения
ULProperty<int, TPySegmenterTrainer> TrainingSize;

/// Выходной размер (необязательные данные)
ULProperty<std::vector<int>, TPySegmenterTrainer> OutputSize;

/// количество классов
ULProperty<int, TPySegmenterTrainer> NumClasses;

/// Названия классов
ULProperty<std::vector<std::string>, TPySegmenterTrainer> Classes;

/// Надо ли разбивать базу на train val test
ULProperty<bool, TPySegmenterTrainer> SplitBase;

/// Путь к обучающему списку (если нет, оставляем "")
ULProperty<std::string, TPySegmenterTrainer> TrainList;

/// Путь к валидационному списку (если нет, оставляем "")
ULProperty<std::string, TPySegmenterTrainer> ValList;

/// Путь к тестовому списку (если нет, оставляем "")
ULProperty<std::string, TPySegmenterTrainer> TestList;

/// Количество изображений в батче
ULProperty<int, TPySegmenterTrainer> BatchSize;

/// Количество батчей в эпохе (TODO вектор с 3-мя значениями?)
ULProperty<int, TPySegmenterTrainer> BatchesInEpoch;

// Переменные состояния
/// Ошибка на тестовом подмножестве
ULProperty<float, TPyBaseTrainer, ptPubState> TestLoss;

/// Точность на тестовом подмножестве
ULProperty<float, TPyBaseTrainer, ptPubState> TestAcc;

/// Ошибка на валидационном подмножестве
ULProperty<float, TPyBaseTrainer, ptPubState> ValLoss;

/// Точность на валидационном подмножестве
ULProperty<float, TPyBaseTrainer, ptPubState> ValAcc;

/// Скорость работы модели
ULProperty<float, TPyBaseTrainer, ptPubState> FPS;

protected: // Временные переменные



public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPySegmenterTrainer(void);
virtual ~TPySegmenterTrainer(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TPySegmenterTrainer* New(void);
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
//virtual bool APyReset(void);

// Выполняет расчет этого объекта
virtual bool ACalculate(void);


// Проверяет входные параметры перед запуском python-функции
virtual bool CheckInputParameters();
// --------------------------
};


}

#endif



