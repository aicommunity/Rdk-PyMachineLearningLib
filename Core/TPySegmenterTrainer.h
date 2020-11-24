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


/// Путь к конфигурационному файлу
ULProperty<std::string, TPySegmenterTrainer> Config;

/// Тип базы из возможных значений: "txt_data", "split_data", "not_split_data".
ULProperty<std::string, TPySegmenterTrainer> DatasetType;

/// Имя набора данных для сохранения в файле конфига и названии весовых коэффициентов
ULProperty<std::string, TPySegmenterTrainer> DatasetName;

/// Размер, на котором будут обучаться изображения, формат: [height,width,channels].
ULProperty<std::vector<int>, TPySegmenterTrainer> InputRes;

/// Названия классов
ULProperty<std::vector<std::string>, TPySegmenterTrainer> Classes;

/// Количество изображений в батче
ULProperty<int, TPySegmenterTrainer> BatchSize;

/// Количество батчей в эпохе
ULProperty<int, TPySegmenterTrainer> BatchesInEpoch;


// Переменные состояния
/// Ошибка на обучающем подмножестве
ULProperty<float, TPyBaseTrainer, ptPubState> TrainLoss;

/// Точность на обучающем подмножестве
ULProperty<float, TPyBaseTrainer, ptPubState> TrainAcc;

/// Ошибка на валидационном подмножестве
ULProperty<float, TPyBaseTrainer, ptPubState> ValLoss;

/// Точность на валидационном подмножестве
ULProperty<float, TPyBaseTrainer, ptPubState> ValAcc;

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



