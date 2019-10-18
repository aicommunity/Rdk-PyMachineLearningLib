#ifndef RDK_TPyObjectDetectorSqueezeDetH
#define RDK_TPyObjectDetectorSqueezeDetH

#include "TPyObjectDetector.h"

namespace RDK {

class TPyObjectDetectorSqueezeDet: public TPyObjectDetector
{
public: // Свойства

///Путь к файлу конфигурации
ULProperty<std::string,TPyObjectDetectorSqueezeDet, ptPubParameter> ConfigPath;

///Путь к файлу весов
ULProperty<std::string,TPyObjectDetectorSqueezeDet, ptPubParameter> WeightsPath;

protected: // Переменные состояния

std::vector<std::string> ClassedList;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyObjectDetectorSqueezeDet(void);
virtual ~TPyObjectDetectorSqueezeDet(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TPyObjectDetectorSqueezeDet* New(void);
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



