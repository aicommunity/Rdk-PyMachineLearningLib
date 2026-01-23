# TPyDetPredict — предикт детекции через Python

## RU

### Назначение

**Класс**: `TPyDetPredict` — компонент для пакетного предсказания детекции на множестве изображений через Python в отдельном потоке.  
**Регистрация**: `Core/Lib.cpp` → `UploadClass("TPyDetPredict", "PyDetPredict")`.  
**Storage-инстансы**: `ClassName = "PyDetPredict"` в конфигурационных проектах.

`TPyDetPredict` выполняет детекцию объектов на всех изображениях из указанной директории используя обученную модель. Работает асинхронно в отдельном Python потоке, что позволяет продолжать работу движка во время обработки.

**Использование:** Пакетная детекция объектов на множестве изображений

### UML-диаграмма классов

```mermaid
classDiagram
    TPyComponent <|-- TPyDetPredict
    class TPyComponent {
        +PythonScriptFileName : string
        +PythonModuleName : string
        +PythonClassName : string
        +UseFullPath : bool
    }
    class TPyDetPredict {
        +WorkingDir : string
        +ImagesDir : string
        +WeightPath : string
        +ConfigPath : string
        +StopNow : bool
        +StartPredict : bool
        +PredictStatus : int
        +ThreadIsAlive : bool
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
- `TPyDetPredict` — предикт детекции

### UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Predict as TPyDetPredict
    participant Python as Python Thread
    participant Instance as Python Instance
    
    Storage->>Predict: New()
    Storage->>Predict: Default()
    Predict->>Predict: APyDefault()
    Note over Predict: PythonModuleName="detection_train"<br/>PythonClassName="DetectionInterface"<br/>WorkingDir=""<br/>ImagesDir=""<br/>StartPredict=false<br/>PredictStatus=0
    Storage->>Predict: SetWorkingDir("Results/")
    Storage->>Predict: SetImagesDir("images/")
    Storage->>Predict: SetConfigPath("config.cfg")
    Storage->>Predict: SetWeightPath("weights.weights")
    Storage->>Predict: Build()
    Predict->>Predict: ABuild()
    Predict->>Predict: PythonInitialize()
    Predict->>Instance: Создание экземпляра
    Predict-->>Storage: Ready = true
    
    Storage->>Predict: StartPredict = true
    loop Каждый шаг расчета
        Storage->>Predict: Calculate()
        Predict->>Predict: ACalculate()
        alt StartPredict && PredictStatus == 0
            Predict->>Predict: CheckInputParameters()
            Predict->>Instance: predict_from_folder(params)
            Note over Python: Запуск предсказания в потоке
            Predict->>Predict: PredictStatus = 1
        end
        
        alt PredictStatus == 1
            Predict->>Instance: is_predicting()
            Instance-->>Predict: PredictStatus
            Predict->>Instance: get_thread_is_alive()
            Instance-->>Predict: ThreadIsAlive
            alt StopNow == true
                Predict->>Instance: stop_now()
                Predict->>Predict: StopNow = false
            end
        end
        
        alt PredictStatus == -1
            Predict->>Instance: get_error_string()
            Instance-->>Predict: ErrorString
            Predict->>Predict: PredictStatus = 0
        end
        
        alt PredictStatus == 2
            Note over Predict: Предсказание завершено успешно
            Predict->>Predict: PredictStatus = 0
        end
    end
    
    Storage->>Predict: Reset()
    Predict->>Predict: APyReset()
    Predict->>Instance: stop_now()
    loop Пока ThreadIsAlive
        Predict->>Instance: get_thread_is_alive()
        Instance-->>Predict: ThreadIsAlive
        Note over Predict: Ожидание завершения потока
    end
```

### UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> Uninitialized: New()
    Uninitialized --> Defaulted: Default()
    Defaulted --> Configuring: SetWorkingDir()<br/>SetImagesDir()<br/>SetConfigPath()<br/>SetWeightPath()
    Configuring --> Building: Build()
    Building --> PythonReady: PythonInitialize()
    PythonReady --> Built: APyBuild()
    Built --> Ready: Ready = true
    Ready --> Idle: PredictStatus = 0
    Idle --> Starting: StartPredict = true
    Starting --> Validating: CheckInputParameters()
    Validating --> Predicting: Параметры валидны<br/>Запуск предсказания
    Validating --> Idle: Параметры невалидны
    Predicting --> Running: PredictStatus = 1
    Running --> Completed: PredictStatus = 2
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
    ParamsValid -->|Да| PrepareParams[Подготовка параметров Python]
    PrepareParams --> CallPredict[predict_from_folder params]
    CallPredict --> CheckStatusAfter{PredictStatus == -1?}
    CheckStatusAfter -->|Да| LogError[LogError: Exception during start]
    LogError --> SetStartFalse
    CheckStatusAfter -->|Нет| SetStatus1[PredictStatus = 1]
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
    SetStatusNull2 --> End
```

### UML-диаграмма компонентов

```mermaid
graph TB
    subgraph TPyComponentBase["TPyComponent"]
        PyComponent[TPyComponent]
    end
    
    subgraph TPyDetPredict["TPyDetPredict"]
        DetPredict[TPyDetPredict]
        PredictionManager[Prediction Manager]
        ThreadManager[Thread Manager]
    end
    
    subgraph PythonEnv["Python Environment"]
        PythonThread[Python Prediction Thread]
        DetectionModule[detection_train Module]
        DetectionInterface[DetectionInterface]
        PredictFunction[predict_from_folder function]
    end
    
    subgraph FileSystem["File System"]
        ImagesDir[Images Directory]
        WorkingDir[Working Directory]
        ConfigFile[Config File]
        WeightsFile[Weights File]
    end
    
    PyComponent -->|inherits| DetPredict
    DetPredict -->|uses| PredictionManager
    DetPredict -->|uses| ThreadManager
    PredictionManager -->|manages| PythonThread
    PythonThread -->|runs| DetectionModule
    DetectionModule -->|contains| DetectionInterface
    DetectionInterface -->|calls| PredictFunction
    PredictFunction -->|reads| ImagesDir
    PredictFunction -->|reads| ConfigFile
    PredictFunction -->|reads| WeightsFile
    PredictFunction -->|writes| WorkingDir
```

### Свойства

#### Параметры (ptPubParameter)

- **`WorkingDir`** (string) — рабочая директория для сохранения результатов предсказания. Автоматически устанавливается в `GetCurrentDataDir() + "Results/"` если не задана. Значение по умолчанию: пустая строка

- **`ImagesDir`** (string) — директория с изображениями для обработки. Должна быть непустой. Значение по умолчанию: пустая строка

- **`WeightPath`** (string) — путь к файлу весов обученной модели. Должен быть непустым. Значение по умолчанию: пустая строка

- **`ConfigPath`** (string) — путь к конфигурационному файлу модели. Должен быть непустым. Значение по умолчанию: пустая строка

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

### Методы

#### Защищенные методы жизненного цикла

- **`APythonInitialize()`** → `bool` — инициализация Python. Всегда возвращает `true`

- **`APyDefault()`** → `bool` — установка значений по умолчанию:
  - `PythonModuleName = "detection_train"`
  - `PythonClassName = "DetectionInterface"`
  - `WorkingDir = ""`
  - `ImagesDir = ""`
  - `ConfigPath = ""`
  - `WeightPath = ""`
  - `StopNow = false`
  - `StartPredict = false`
  - `PredictStatus = 0`
  - `ThreadIsAlive = false`

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
  - Всегда возвращает `true`

- **`CheckInputParameters()`** → `bool` — проверка валидности входных параметров:
  - Проверяет, что `WorkingDir` не пуст
  - Проверяет, что `ConfigPath` не пуст
  - Проверяет, что `WeightPath` не пуст
  - Проверяет, что `ImagesDir` не пуст
  - Возвращает `true` если все параметры валидны, `false` иначе

### Примеры использования

#### Пример 1: C++ код

```cpp
auto predictor = storage->CreateComponent<TPyDetPredict>();
predictor->SetPythonScriptFileName("detection_train.py");
predictor->WorkingDir = "Results/";
predictor->ImagesDir = "test_images/";
predictor->ConfigPath = "yolo.cfg";
predictor->WeightPath = "yolo.weights";
predictor->Build();

// Запуск предсказания
predictor->Reset();
predictor->StartPredict = true;

// Мониторинг
for (int step = 0; step < numSteps; step++) {
    predictor->Calculate();
    if (predictor->PredictStatus == 1) {
        std::cout << "Prediction in progress..." << std::endl;
    }
    if (predictor->PredictStatus == 2) {
        std::cout << "Prediction completed!" << std::endl;
        break;
    }
    if (predictor->PredictStatus == -1) {
        std::cout << "Prediction error!" << std::endl;
        break;
    }
}
```

#### Пример 2: XML конфигурация

```xml
<Predictor ClassName="PyDetPredict">
    <Property Name="PythonScriptFileName" Value="detection_train.py" />
    <Property Name="WorkingDir" Value="Results/" />
    <Property Name="ImagesDir" Value="test_images/" />
    <Property Name="ConfigPath" Value="yolo.cfg" />
    <Property Name="WeightPath" Value="yolo.weights" />
    <Property Name="StartPredict" Value="true" />
</Predictor>
```

### Особенности работы

1. **Асинхронное выполнение**: Предсказание выполняется в отдельном Python потоке, что позволяет продолжать работу движка

2. **Автоматическое управление директориями**: `WorkingDir` автоматически устанавливается в `DataDir + "Results/"` если не задана явно

3. **Безопасная остановка**: При деструкторе компонент гарантированно дожидается завершения Python потока

4. **Мониторинг**: Статус предсказания обновляется в каждом вызове `ACalculate()`

### Связи с другими компонентами

- **Использует**: Python модуль `detection_train` с классом `DetectionInterface`
- **Типичное использование**: После обучения детектора (`TPyDetectorTrainer`) для пакетной обработки тестовых изображений

### См. также

- [`TPyDetectorTrainer`](TPyDetectorTrainer.md) — тренер детекторов
- [`TPyObjectDetector`](TPyObjectDetector.md) — детектор объектов
- [`TPyPredictSort`](TPyPredictSort.md) — сортировка предсказаний
- [Architecture.md](../Architecture.md) — архитектура библиотеки

---

## EN

### Purpose

**Class**: `TPyDetPredict` — component for batch detection prediction on multiple images via Python in a separate thread.  
**Registration**: `Core/Lib.cpp` → `UploadClass("TPyDetPredict", "PyDetPredict")`.  
**Instances**: `ClassName = "PyDetPredict"` in configuration projects.

`TPyDetPredict` performs object detection on all images from specified directory using trained model. Works asynchronously in a separate Python thread.

**Usage:** Batch object detection on multiple images

### See also

- [`TPyDetectorTrainer`](TPyDetectorTrainer.md) — detector trainer
- [`TPyObjectDetector`](TPyObjectDetector.md) — object detector
- [`TPyPredictSort`](TPyPredictSort.md) — prediction sorting
- [Architecture.md](../Architecture.md) — library architecture
