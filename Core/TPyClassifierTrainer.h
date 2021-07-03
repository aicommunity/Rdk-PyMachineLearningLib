#ifndef RDK_TPyClassifierTrainerH
#define RDK_TPyClassifierTrainerH

#include "TPyBaseTrainer.h"



namespace RDK {

class TPyClassifierTrainer: public TPyBaseTrainer
{
public: // Свойства  

/// Имя набора данных для сохранения в файле конфига и названии весов
ULProperty<std::string, TPyClassifierTrainer> DatasetName;

/// Копировать ли в рабочую директорию картинки разбитые на train, val, test
/// согласно сгенерированным разбиениям из текстовых файлов
ULProperty<bool, TPyClassifierTrainer> CopySplittedImages;

/// Стоит ли приравнивать валидационную выборку тестовой.
/// В случае если разбиение равно 70, 20, 10 и TestEqualVal=True, то 10% выборки не будет использовано.
ULProperty<bool, TPyClassifierTrainer> TestEqualVal;

/// Параметры изображения (width, height, channels)
ULProperty<std::vector<int>, TPyClassifierTrainer> ImageSize;

/// Learning Rate
ULProperty<float, TPyClassifierTrainer> LearningRate;

/// Размеры трех batch-ей, для train, val и test
ULProperty<std::vector<int>, TPyClassifierTrainer> BatchSizes;

/// Количество тренируемых слоев. N последних
ULProperty<int, TPyClassifierTrainer> LayersToBeTrained;

/// Классы, опционально(список), если учимся не на всех классах датасета
ULProperty<std::vector<std::string>, TPyClassifierTrainer> Classes;

/// Текущая ошибка на тренировочном наборе
ULProperty<float, TPyBaseTrainer, ptPubState> TrainLoss;

/// Текущая точность на тренировочном наборе
ULProperty<float, TPyBaseTrainer, ptPubState> TrainAcc;

/// Текущая ошибка на валидационном наборе
ULProperty<float, TPyBaseTrainer, ptPubState> ValLoss;

/// Текущая точность на валидационном наборе
ULProperty<float, TPyBaseTrainer, ptPubState> ValAcc;

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
//virtual bool APyReset(void);

// Выполняет расчет этого объекта
virtual bool ACalculate(void);


// Проверяет входные параметры перед запуском python-функции
virtual bool CheckInputParameters();
// --------------------------
};


}

#endif



