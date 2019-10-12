#ifndef RDK_TPyObjectDetectorYoloH
#define RDK_TPyObjectDetectorYoloH

#include "TPyObjectDetector.h"

namespace RDK {

#define YOLOV2_INITTYPE 2
#define YOLOV3_INITTYPE 3

class TPyObjectDetectorYolo: public TPyObjectDetector
{
public: // Свойства
///Тип инициализации: 2 - YOLOV2
///                   3 - YOLOV3
ULProperty<int,TPyObjectDetectorYolo, ptPubParameter> InitializationTypeYOLO;

///Путь к файлу конфигурации Йолы
ULProperty<std::string,TPyObjectDetectorYolo, ptPubParameter> ConfigPathYOLO;

///Путь к файлу весов Йолы
ULProperty<std::string,TPyObjectDetectorYolo, ptPubParameter> WeightsPathYOLO;

protected: // Переменные состояния

std::vector<std::string> ClassedList;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyObjectDetectorYolo(void);
virtual ~TPyObjectDetectorYolo(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TPyObjectDetectorYolo* New(void);
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
bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
// --------------------------
};


}

#endif



