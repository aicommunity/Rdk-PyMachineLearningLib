#ifndef TPyObjectDetectorYoloExH
#define RDK_TPyObjectDetectorYoloExH

#include "TPyObjectDetector.h"

namespace RDK {

class TPyObjectDetectorYoloEx: public TPyObjectDetector
{
public: // Свойства
///Путь к модели Йолы
ULProperty<std::string,TPyObjectDetectorYoloEx, ptPubParameter> ModelPathYOLO;

///Путь к файлу с якорями (оставить пустым если используем стандарт)
ULProperty<std::string,TPyObjectDetectorYoloEx, ptPubParameter> AnchorsPathYOLO;

///Путь к файлу с перечнем классов для используемой сети
ULProperty<std::string,TPyObjectDetectorYoloEx, ptPubParameter> ClassesPathYOLO;

///Список целевых классов
ULProperty<std::vector<std::string>,TPyObjectDetectorYoloEx, ptPubParameter> TargetClassesYOLO;

///Загружать список классов из соответствующего файла
ULProperty<bool,TPyObjectDetectorYoloEx, ptPubParameter> LoadTargetClassesYOLO;

ULProperty<int,TPyObjectDetectorYoloEx, ptPubParameter> NumTargetClassesYOLO;
ULProperty<int,TPyObjectDetectorYoloEx, ptPubParameter>  NumChangeClassesYOLO;

///Список замен классов (изменить список классов в файле ClassesPathYOLO на свои имена)
///ВНИМАНИЕ!!! Число классов должно соответствовать числу классов в ClassesPathYOLO
ULProperty<std::vector<std::string>,TPyObjectDetectorYoloEx, ptPubParameter> ChangeClassesYOLO;

protected: // Временные переменные

std::vector<std::string> ClassedList;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyObjectDetectorYoloEx(void);
virtual ~TPyObjectDetectorYoloEx(void);
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
bool SetNumTargetClassesYOLO(const int& num);

bool SetNumChangeClassesYOLO(const int& num);
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TPyObjectDetectorYoloEx* New(void);
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
protected:
bool APythonInitialize(void);

// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool APyDefault2(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool APyBuild2(void);

// Сброс процесса счета без потери настроек
virtual bool APyReset2(void);

// Выполняет обнаружение
virtual bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
// --------------------------
};


}

#endif



