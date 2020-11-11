#ifndef RDK_TPyComponentH
#define RDK_TPyComponentH

#include "TPythonIntegrationInclude.h"
#include "TPythonIntegrationUtil.h"

// кастомные дефайны для управления испонением потоков питона
// исходное состояние для любого компонента с питоном - это Py_CUSTOM_BLOCK_THREADS (то есть дестркуторах и конструкторах вызывается)
// оригинал в boost Py_BLOCK_THREADS и Py_UNBLOCK_THREADS
#define Py_CUSTOM_BLOCK_THREADS        if(_custom_save!=nullptr){PyEval_RestoreThread(_custom_save);\
                                                                _custom_save=nullptr;}

#define Py_CUSTOM_UNBLOCK_THREADS      if(_custom_save==nullptr){_custom_save = PyEval_SaveThread();}


namespace RDK {

class TPyComponent: virtual public RDK::UNet
{
public: // Свойства
/// Имя файла выполняемого скрипта, лежащего относительно папки конфигурации
ULProperty<std::string, TPyComponent> PythonScriptFileName;

/// Deprecated: PythonScriptPath == PythonScriptFileName
//ULProperty<std::string, TPyComponent> PythonScriptPath;

/// Имя питон-модуля
ULProperty<std::string, TPyComponent> PythonModuleName;

/// Имя питон-класса
ULProperty<std::string, TPyComponent> PythonClassName;

/// Использовать путь к файлу "как есть". Вкл по умолчанию, надо переключать для старых проектов
ULProperty<bool, TPyComponent> UseFullPath;

protected: // Временные переменные
//Состояние потока
//Нужен чтобы отключать/включать исполнение потоков питона
static PyThreadState *_custom_save;

/// Флаг взводится при успешной инициализации подсистемы питона
bool PythonInitialized;

/// Интерфейс 1
boost::python::object IntegrationInterface;

/// Интерфейс 2
boost::python::object IntegrationInterfaceInstance;

/// Полный путь до имени скрипта
std::string FullPythonScriptFileName;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TPyComponent(void);
virtual ~TPyComponent(void);
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
bool SetPythonScriptFileName(const std::string& path);
//bool SetPythonClassifierScriptPath(const std::string& path);
bool SetPythonModuleName(const std::string& path);
bool SetPythonClassName(const std::string& path);
// ---------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
protected:
virtual void PythonInitialize(void);

/// Обязательно возвращает true если успешно завершился
/// и false в случае неудачи!
virtual bool APythonInitialize(void)=0;

// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);
virtual bool APyDefault(void)=0;

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool ABuild(void);
virtual bool APyBuild(void)=0;

// Сброс процесса счета без потери настроек
virtual bool AReset(void);
virtual bool APyReset(void)=0;

// Выполняет расчет этого объекта
virtual bool ABeforeCalculate(void);

// --------------------------
};

}

#endif
