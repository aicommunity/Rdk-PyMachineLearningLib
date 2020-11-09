#ifndef RDK_TPyDetectorTrainerH
#define RDK_TPyDetectorTrainerH

#include "TPyBaseTrainer.h"




namespace RDK {

class TPyDetectorTrainer: public TPyBaseTrainer
{
public: // Свойства  

/// Тип набора данных на данный момент список возможных значений: "xml_main_dirs", "xml_txt_splits", "xml_txt_main_lists", "xml_test_dirs", "xml_txt_test_splits"
ULProperty<std::string, TPyDetectorTrainer, ptPubParameter> DatasetType;

/// Конфиг с общими и специфическими параметрами для архитектуры
ULProperty<std::string, TPyDetectorTrainer, ptPubParameter> Config;

/// Сохранять ли на этапе тестирования в подпапку /pred рабочей директории
/// xml файлы содержащие предсказания нейросети на тестовом наборе данных
ULProperty<bool, TPyDetectorTrainer, ptPubParameter> SavePredicted;

/// Сохраняет в подпапку /images рабочей директории изображения с визуализациями обнаружений нейронной сети.
/// Возможные параметры: 0 (false) или 9999 ("all") или другое целое число
/// Если задано число n, то равномерно выбирается n картинок из тестового набора данных 
ULProperty<int, TPyDetectorTrainer, ptPubParameter> Visualize;

/// Рисовать ли ограничивающие рамки для аннотаций на примерах обнаружения при активном параметре visualize
/// (пусть аннотации рисуются белым всегда) может быть полезно при анализе полученных показателей точности,
/// однако обычно сильно ухудшает наглядность необходимую для демонстрации
ULProperty<bool, TPyDetectorTrainer, ptPubParameter> PaintGt;

// Переменные состояния
/// Список названий функций потерь для конкретной архитектуры
ULProperty<std::vector<std::string>, TPyDetectorTrainer, ptPubState> LossNames;

/// Список названий функций потерь
ULProperty<std::vector<float>, TPyDetectorTrainer, ptPubParameter> TrainLosses;

/// Список названий функций
ULProperty<std::vector<float>, TPyDetectorTrainer, ptPubParameter> ValLosses;

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
//virtual bool APyReset(void);

// Выполняет расчет этого объекта
virtual bool ACalculate(void);


// Проверяет входные параметры перед запуском python-функции
bool CheckInputParameters();
// --------------------------
};


}

#endif



