#ifndef RDK_TPySegmentatorProtobufH
#define RDK_TPySegmentatorProtobufH

#include "TPySegmentator.h"

namespace RDK {

class TPySegmentatorProtobuf: public TPySegmentator
{
public: // Свойства

///Путь к файлу конфигурации
ULProperty<std::string,TPySegmentatorProtobuf, ptPubParameter> JSONPath;

///Путь к файлу весов
ULProperty<std::string,TPySegmentatorProtobuf, ptPubParameter> ProtobufPath;

protected: // Переменные состояния
public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPySegmentatorProtobuf(void);
virtual ~TPySegmentatorProtobuf(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TPySegmentatorProtobuf* New(void);
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

// Выполняет сегментацию
bool Inference(UBitmap &bmp, UBitmap &mask);
// --------------------------
};


}

#endif



