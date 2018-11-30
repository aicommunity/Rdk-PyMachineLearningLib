#ifndef RDK_TPyUBitmapClassifierH
#define RDK_TPyUBitmapClassifierH

#include "TPyComponent.h"

namespace RDK {

class TPyUBitmapClassifier: public TPyComponent
{
public: // Свойства
/// Входное изображение
/// UPropertyInputData<UBitmap,TPyUBitmapClassifier> InputImage;

//Входные матрицы с данными об обнаружениях
/// Содержит изображения (обработанные) для классификации
UPropertyInputData<std::vector<UBitmap>, TPyUBitmapClassifier, ptPubInput> InputImages;

/// Целое число, определяющее цветовую модель, на которую рассчитана сеть
/// ubmRGB24=3 - цветное изображение
/// umbY8=400 - черно-белое изображение
ULProperty<int,TPyUBitmapClassifier, ptPubParameter> ImageColorModel;

/// Количество классов объектов (какой размер будет у вектора
ULProperty<int,TPyUBitmapClassifier, ptPubParameter> NumClasses;

/// Выходная матрица с классами объектов
UPropertyOutputData<std::vector<int>,TPyUBitmapClassifier, ptPubOutput> OutputClasses;

/// Выходная матрица. Количество столбцов по числу объектов, количество строк в столбце по числу классов
/// Каждое значение - уверенность класса
UPropertyOutputData<MDMatrix<double>, TPyUBitmapClassifier> OutputConfidences;

protected: // Переменные состояния

UGraphics Graph;
UBitmap Canvas;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyUBitmapClassifier(void);
virtual ~TPyUBitmapClassifier(void);
// --------------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TPyUBitmapClassifier* New(void);
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
// --------------------------
};


}

#endif



