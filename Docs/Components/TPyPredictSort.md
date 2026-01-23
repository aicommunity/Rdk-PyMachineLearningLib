# TPyPredictSort — сортировка предсказаний через Python

## RU

### Назначение

**Класс**: `TPyPredictSort` — компонент для предсказания и сортировки изображений по классам через Python в отдельном потоке.  
**Регистрация**: `Core/Lib.cpp` → `UploadClass("TPyPredictSort", "PyPredictSort")`.  
**Storage-инстансы**: `ClassName = "PyPredictSort"` в конфигурационных проектах.

`TPyPredictSort` выполняет классификацию изображений из указанной директории и сортирует их по классам в отдельные папки. Работает асинхронно в отдельном Python потоке, что позволяет продолжать работу движка во время обработки.

**Использование:** Пакетная классификация и сортировка изображений по классам

### UML-диаграмма классов

```mermaid
classDiagram
    TPyComponent <|-- TPyPredictSort
    class TPyComponent {
        +PythonScriptFileName : string
        +PythonModuleName : string
        +PythonClassName : string
        +UseFullPath : bool
    }
    class TPyPredictSort {
        +WorkingDir : string
        +ImagesDir : string
        +WeightPath : string
        +ConfigPath : string
        +SortImages : bool
        +StopNow : bool
        +StartPredict : bool
        +PredictStatus : int
        +ThreadIsAlive : bool
        +PredictionEnded : bool
        +ACalculate() bool
        +CheckInputParameters() bool
        +APythonInitialize() bool
        +APyDefault() bool
        +APyBuild() bool
        +APyReset() bool
    }
```

**Иерархия наследования:**
- `TPyComponent` — базовый Python-компонент
- `TPyPredictSort` — сортировка предсказаний

### UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Sorter as TPyPredictSort
    participant Python as Python Thread
    participant Instance as Python Instance
    
    Storage->>Sorter: New()
    Storage->>Sorter: Default()
    Sorter->>Sorter: APyDefault()
    Note over Sorter: PythonModuleName="classifier_interface_tf1"<br/>PythonClassName="ClassificationInterface"<br/>WorkingDir=""<br/>ImagesDir=""<br/>SortImages=false<br/>StartPredict=false<br/>PredictStatus=0<br/>PredictionEnded=false
    Storage->>Sorter: SetWorkingDir("Results/")
    Storage->>Sorter: SetImagesDir("images/")
    Storage->>Sorter: SetConfigPath("config.json")
    Storage->>Sorter: SetWeightPath("model.h5")
    Storage->>Sorter: SetSortImages(true)
    Storage->>Sorter: Build()
    Sorter->>Sorter: ABuild()
    Sorter->>Sorter: PythonInitialize()
    Sorter->>Instance: Создание экземпляра
    Sorter-->>Storage: Ready = true
    
    Storage->>Sorter: StartPredict = true
    loop Каждый шаг расчета
        Storage->>Sorter: Calculate()
        Sorter->>Sorter: ACalculate()
        alt StartPredict && PredictStatus == 0
            Sorter->>Sorter: CheckInputParameters()
            Sorter->>Instance: predict_and_sort_wrapper(config, weights, params)
            Note over Python: Запуск предсказания и сортировки в потоке
            Sorter->>Sorter: PredictStatus = 1
            Sorter->>Sorter: PredictionEnded = false
        end
        
        alt PredictStatus == 1
            Sorter->>Instance: is_predicting()
            Instance-->>Sorter: PredictStatus
            Sorter->>Instance: get_thread_is_alive()
            Instance-->>Sorter: ThreadIsAlive
            alt StopNow == true
                Sorter->>Instance: stop_now()
                Sorter->>Sorter: StopNow = false
            end
        end
        
        alt PredictStatus == -1
            Sorter->>Instance: get_error_string()
            Instance-->>Sorter: ErrorString
            Sorter->>Sorter: PredictStatus = 0
        end
        
        alt PredictStatus == 2
            Note over Sorter: Предсказание и сортировка завершены успешно
            Sorter->>Sorter: PredictStatus = 0
            Sorter->>Sorter: PredictionEnded = true
        end
    end
```

### UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> Uninitialized: New()
    Uninitialized --> Defaulted: Default()
    Defaulted --> Configuring: SetWorkingDir()<br/>SetImagesDir()<br/>SetConfigPath()<br/>SetWeightPath()<br/>SetSortImages()
    Configuring --> Building: Build()
    Building --> PythonReady: PythonInitialize()
    PythonReady --> Built: APyBuild()
    Built --> Ready: Ready = true
    Ready --> Idle: PredictStatus = 0
    Idle --> Starting: StartPredict = true
    Starting --> Validating: CheckInputParameters()
    Validating --> Predicting: Параметры валидны<br/>Запуск предсказания
    Validating --> Idle: Параметры невалидны
    Predicting --> Running: PredictStatus = 1<br/>PredictionEnded = false
    Running --> Completed: PredictStatus = 2<br/>PredictionEnded = true
    Running --> Error: PredictStatus = -1
    Running --> Stopping: StopNow = true
    Stopping --> Idle: Немедленная остановка
    Error --> Idle: Сброс статуса
    Completed --> Idle: Сброс статуса
    Ready --> Resetting: Reset()
    Resetting --> StoppingThread: stop_now()
    StoppingThread --> WaitingThread: Ожидание завершения потока
    WaitingThread --> Idle: ThreadIsAlive = false
```

### UML-диаграмма активности

```mermaid
flowchart TD
    Start([Start ACalculate]) --> CheckInit{PythonInitialized?}
    CheckInit -->|Нет| End([End: return true])
    CheckInit -->|Да| UpdateWorkingDir{WorkingDir !=<br/>DataDir+Results/?}
    UpdateWorkingDir -->|Да| SetWorkingDir[WorkingDir = DataDir+Results/]
    UpdateWorkingDir -->|Нет| CheckStatus{PredictStatus?}
    SetWorkingDir --> CheckStatus
    
    CheckStatus -->|0| CheckStart{StartPredict?}
    CheckStart -->|Нет| End
    CheckStart -->|Да| CheckThread{ThreadIsAlive?}
    CheckThread -->|Да| LogWarning[LogWarning: Thread alive]
    LogWarning --> SetStartFalse[StartPredict = false]
    SetStartFalse --> End
    CheckThread -->|Нет| ValidateParams[CheckInputParameters]
    ValidateParams --> ParamsValid{Параметры валидны?}
    ParamsValid -->|Нет| SetStartFalse
    ParamsValid -->|Да| CheckTrailingSlash{ImagesDir<br/>ends with /?}
    CheckTrailingSlash -->|Нет| AddSlash[Add / to ImagesDir]
    CheckTrailingSlash -->|Да| PrepareParams[Подготовка параметров Python]
    AddSlash --> PrepareParams
    PrepareParams --> CallPredict[predict_and_sort_wrapper config weights params]
    CallPredict --> CheckStatusAfter{PredictStatus == -1?}
    CheckStatusAfter -->|Да| LogError[LogError: Exception during start]
    LogError --> SetStartFalse
    CheckStatusAfter -->|Нет| SetStatus1[PredictStatus = 1<br/>PredictionEnded = false]
    SetStatus1 --> SetStartFalse
    
    CheckStatus -->|1| GetStatus[is_predicting]
    GetStatus --> GetThread[get_thread_is_alive]
    GetThread --> CheckStopNow{StopNow?}
    CheckStopNow -->|Да| CallStopNow[stop_now]
    CallStopNow --> SetStopFalse[StopNow = false]
    SetStopFalse --> End
    CheckStopNow -->|Нет| End
    
    CheckStatus -->|-1| GetError[get_error_string]
    GetError --> LogError2[LogWarning: Exception or stop]
    LogError2 --> ResetStatus[PredictStatus = 0]
    ResetStatus --> SetStatusNull[set_predicting_status_to_null]
    SetStatusNull --> End
    
    CheckStatus -->|2| LogComplete[LogInfo: Prediction completed]
    LogComplete --> ResetStatus2[PredictStatus = 0]
    ResetStatus2 --> SetStatusNull2[set_predicting_status_to_null]
    SetStatusNull2 --> SetEnded[PredictionEnded = true]
    SetEnded --> End
```

### UML-диаграмма компонентов

```mermaid
graph TB
    subgraph TPyComponentBase["TPyComponent"]
        PyComponent[TPyComponent]
    end
    
    subgraph TPyPredictSort["TPyPredictSort"]
        PredictSort[TPyPredictSort]
        PredictionManager[Prediction Manager]
        SortingManager[Sorting Manager]
        ThreadManager[Thread Manager]
    end
    
    subgraph PythonEnv["Python Environment"]
        PythonThread[Python Prediction Thread]
        ClassifierModule[classifier_interface_tf1 Module]
        ClassifierInterface[ClassificationInterface]
        PredictSortFunction[predict_and_sort_wrapper function]
    end
    
    subgraph FileSystem["File System"]
        ImagesDir[Images Directory]
        WorkingDir[Working Directory]
        SortedDirs[Sorted Class Directories]
        ConfigFile[Config File]
        WeightsFile[Weights File]
    end
    
    PyComponent -->|inherits| PredictSort
    PredictSort -->|uses| PredictionManager
    PredictSort -->|uses| SortingManager
    PredictSort -->|uses| ThreadManager
    PredictionManager -->|manages| PythonThread
    SortingManager -->|manages| PythonThread
    PythonThread -->|runs| ClassifierModule
    ClassifierModule -->|contains| ClassifierInterface
    ClassifierInterface -->|calls| PredictSortFunction
    PredictSortFunction -->|reads| ImagesDir
    PredictSortFunction -->|reads| ConfigFile
    PredictSortFunction -->|reads| WeightsFile
    PredictSortFunction -->|writes| SortedDirs
    PredictSortFunction -->|uses| WorkingDir
```

### Свойства

#### Параметры (ptPubParameter)

- **`WorkingDir`** (string) — рабочая директория для сохранения результатов. Автоматически устанавливается в `GetCurrentDataDir() + "Results/"` если не задана. Значение по умолчанию: пустая строка

- **`ImagesDir`** (string) — директория с изображениями для обработки. Должна быть непустой. Автоматически добавляется завершающий `/` если отсутствует. Значение по умолчанию: пустая строка

- **`WeightPath`** (string) — путь к файлу весов обученной модели. Должен быть непустым. Значение по умолчанию: пустая строка

- **`ConfigPath`** (string) — путь к конфигурационному файлу модели. Должен быть непустым. Значение по умолчанию: пустая строка

- **`SortImages`** (bool) — сортировать ли изображения по классам в отдельные папки. Если `true`, изображения копируются в папки по классам. Значение по умолчанию: `false`

- **`StopNow`** (bool) — флаг немедленной остановки предсказания. Автоматически сбрасывается после обработки. Значение по умолчанию: `false`

- **`StartPredict`** (bool) — флаг запуска предсказания. Устанавливается в `true` для начала предсказания, автоматически сбрасывается после запуска. Значение по умолчанию: `false`

#### Состояние (ptPubState)

- **`PredictStatus`** (int) — статус предсказания:
  - -1 — ошибка или остановка
  - 0 — не запущено (готов к запуску)
  - 1 — предсказание выполняется
  - 2 — предсказание завершено успешно
  Значение по умолчанию: 0

- **`ThreadIsAlive`** (bool) — флаг активности Python потока предсказания. `true` если поток активен, `false` если завершен. Значение по умолчанию: `false`

- **`PredictionEnded`** (bool) — флаг завершения предсказания. Устанавливается в `true` когда `PredictStatus == 2`. Значение по умолчанию: `false`

### Методы

#### Защищенные методы жизненного цикла

- **`APythonInitialize()`** → `bool` — инициализация Python. Всегда возвращает `true`

- **`APyDefault()`** → `bool` — установка значений по умолчанию:
  - `PythonModuleName = "classifier_interface_tf1"`
  - `PythonClassName = "ClassificationInterface"`
  - `WorkingDir = ""`
  - `ImagesDir = ""`
  - `ConfigPath = ""`
  - `WeightPath = ""`
  - `SortImages = false`
  - `StopNow = false`
  - `StartPredict = false`
  - `PredictStatus = 0`
  - `ThreadIsAlive = false`
  - `PredictionEnded = false`

- **`APyBuild()`** → `bool` — сборка компонента. Всегда возвращает `true`

- **`APyReset()`** → `bool` — сброс состояния:
  - Устанавливает `StopNow = false`, `StartPredict = false`
  - Останавливает Python поток через `stop_now()`
  - Ожидает завершения потока
  - Возвращает `true`

- **`ACalculate()`** → `bool` — выполнение расчета:
  - Обновляет `WorkingDir` если необходимо
  - Проверяет и запускает предсказание при `StartPredict = true`
  - Мониторит статус предсказания
  - Обрабатывает команды остановки
  - Обрабатывает завершение предсказания и ошибки
  - Устанавливает `PredictionEnded = true` при успешном завершении
  - Всегда возвращает `true`

- **`CheckInputParameters()`** → `bool` — проверка валидности входных параметров:
  - Проверяет, что `WorkingDir` не пуст
  - Проверяет, что `ConfigPath` не пуст
  - Проверяет, что `WeightPath` не пуст
  - Проверяет, что `ImagesDir` не пуст
  - Добавляет завершающий `/` к `ImagesDir` если отсутствует
  - Возвращает `true` если все параметры валидны, `false` иначе

### Примеры использования

#### Пример 1: C++ код

```cpp
auto sorter = storage->CreateComponent<TPyPredictSort>();
sorter->SetPythonScriptFileName("classifier_interface_tf1.py");
sorter->WorkingDir = "Results/";
sorter->ImagesDir = "test_images";
sorter->ConfigPath = "config.json";
sorter->WeightPath = "model.h5";
sorter->SortImages = true;
sorter->Build();

// Запуск предсказания и сортировки
sorter->Reset();
sorter->StartPredict = true;

// Мониторинг
for (int step = 0; step < numSteps; step++) {
    sorter->Calculate();
    if (sorter->PredictStatus == 1) {
        std::cout << "Prediction and sorting in progress..." << std::endl;
    }
    if (sorter->PredictStatus == 2 && sorter->PredictionEnded) {
        std::cout << "Prediction and sorting completed!" << std::endl;
        break;
    }
    if (sorter->PredictStatus == -1) {
        std::cout << "Prediction error!" << std::endl;
        break;
    }
}
```

#### Пример 2: XML конфигурация

```xml
<Sorter ClassName="PyPredictSort">
    <Property Name="PythonScriptFileName" Value="classifier_interface_tf1.py" />
    <Property Name="WorkingDir" Value="Results/" />
    <Property Name="ImagesDir" Value="test_images" />
    <Property Name="ConfigPath" Value="config.json" />
    <Property Name="WeightPath" Value="model.h5" />
    <Property Name="SortImages" Value="true" />
    <Property Name="StartPredict" Value="true" />
</Sorter>
```

### Особенности работы

1. **Асинхронное выполнение**: Предсказание и сортировка выполняются в отдельном Python потоке

2. **Автоматическая сортировка**: При `SortImages = true` изображения автоматически копируются в папки по классам

3. **Автоматическое управление директориями**: 
   - `WorkingDir` автоматически устанавливается в `DataDir + "Results/"` если не задана явно
   - `ImagesDir` автоматически получает завершающий `/` если отсутствует

4. **Безопасная остановка**: При деструкторе компонент гарантированно дожидается завершения Python потока

5. **Мониторинг**: Статус предсказания и флаг завершения обновляются в каждом вызове `ACalculate()`

### Связи с другими компонентами

- **Использует**: Python модуль `classifier_interface_tf1` с классом `ClassificationInterface`
- **Типичное использование**: После обучения классификатора (`TPyClassifierTrainer`) для пакетной классификации и сортировки изображений

### См. также

- [`TPyClassifierTrainer`](TPyDetectorTrainer.md#tpyclassifiertrainer) — тренер классификаторов
- [`TPyUBitmapClassifier`](TPyUBitmapClassifier.md) — классификатор изображений
- [`TPyDetPredict`](TPyDetPredict.md) — предикт детекции
- [Architecture.md](../Architecture.md) — архитектура библиотеки

---

## EN

### Purpose

**Class**: `TPyPredictSort` — component for prediction and sorting images by classes via Python in a separate thread.  
**Registration**: `Core/Lib.cpp` → `UploadClass("TPyPredictSort", "PyPredictSort")`.  
**Instances**: `ClassName = "PyPredictSort"` in configuration projects.

`TPyPredictSort` performs classification of images from specified directory and sorts them by classes into separate folders. Works asynchronously in a separate Python thread.

**Usage:** Batch classification and sorting images by classes

### See also

- [`TPyClassifierTrainer`](TPyDetectorTrainer.md#tpyclassifiertrainer) — classifier trainer
- [`TPyUBitmapClassifier`](TPyUBitmapClassifier.md) — image classifier
- [`TPyDetPredict`](TPyDetPredict.md) — detection prediction
- [Architecture.md](../Architecture.md) — library architecture
