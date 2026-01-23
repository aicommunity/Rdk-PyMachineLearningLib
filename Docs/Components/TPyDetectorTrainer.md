# TPyDetectorTrainer / TPySegmenterTrainer / TPyClassifierTrainer — тренеры через Python

## RU

### Назначение

**Классы**: 
- `TPyClassifierTrainer` — тренер классификаторов через Python
- `TPyDetectorTrainer` — тренер детекторов через Python  
- `TPySegmenterTrainer` — тренер сегментаторов через Python

**Регистрация**: `Core/Lib.cpp` → `UploadClass("TPyClassifierTrainer", ...)`, `UploadClass("TPyDetectorTrainer", ...)`, `UploadClass("TPySegmenterTrainer", ...)`.  
**Storage-инстансы**: Соответствующие `ClassName` в конфигурационных проектах.

Все тренеры наследуются от `TPyBaseTrainer` и предоставляют специализированную функциональность для обучения соответствующих типов моделей машинного обучения.

**Использование:** Обучение моделей классификации, детекции и сегментации через Python

### UML-диаграмма классов

```mermaid
classDiagram
    TPyBaseTrainer <|-- TPyClassifierTrainer
    TPyBaseTrainer <|-- TPyDetectorTrainer
    TPyBaseTrainer <|-- TPySegmenterTrainer
    class TPyBaseTrainer {
        +TrainDataDir : vector~string~
        +WorkingDir : string
        +ArchitectureName : string
        +Epochs : int
        +StartTraining : bool
        +TrainingStatus : int
        +Epoch : int
        +Progress : float
        +ACalculate() bool
        #CheckInputParameters() bool*
    }
    class TPyClassifierTrainer {
        +DatasetName : string
        +CopySplittedImages : bool
        +TestEqualVal : bool
        +ImageSize : vector~int~
        +LearningRate : float
        +BatchSizes : vector~int~
        +LayersToBeTrained : int
        +Classes : vector~string~
        +TrainLoss : float
        +TrainAcc : float
        +ValLoss : float
        +ValAcc : float
        +ACalculate() bool
        +CheckInputParameters() bool
    }
    class TPyDetectorTrainer {
        +DatasetType : string
        +Config : string
        +SavePredicted : bool
        +Visualize : int
        +PaintGt : bool
        +LossNames : vector~string~
        +TrainLosses : vector~double~
        +ValLosses : vector~double~
        +ACalculate() bool
        +CheckInputParameters() bool
    }
    class TPySegmenterTrainer {
        +Config : string
        +DatasetType : string
        +DatasetName : string
        +InputRes : vector~int~
        +Classes : vector~string~
        +BatchSize : int
        +BatchesInEpoch : int
        +TrainLoss : float
        +TrainAcc : float
        +ValLoss : float
        +ValAcc : float
        +ACalculate() bool
        +CheckInputParameters() bool
    }
```

**Иерархия наследования:**
- `TPyBaseTrainer` — базовый класс тренера
- `TPyClassifierTrainer` — тренер классификаторов
- `TPyDetectorTrainer` — тренер детекторов
- `TPySegmenterTrainer` — тренер сегментаторов

### UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Trainer as TPyClassifierTrainer
    participant Python as Python Thread
    participant Instance as Python Instance
    
    Storage->>Trainer: New()
    Storage->>Trainer: Default()
    Trainer->>Trainer: APyDefault()
    Note over Trainer: PythonModuleName="classifier_interface_tf1"<br/>PythonClassName="ClassificationInterface"<br/>DatasetName="dataset_ft"<br/>ImageSize={224,224,3}<br/>LearningRate=0.0002
    Storage->>Trainer: SetTrainDataDir(...)
    Storage->>Trainer: SetDatasetName(...)
    Storage->>Trainer: SetArchitectureName(...)
    Storage->>Trainer: Build()
    Trainer->>Trainer: ABuild()
    Trainer->>Trainer: PythonInitialize()
    Trainer-->>Storage: Ready = true
    
    Storage->>Trainer: StartTraining = true
    loop Каждый шаг расчета
        Storage->>Trainer: Calculate()
        Trainer->>Trainer: ACalculate()
        alt StartTraining && TrainingStatus == 0
            Trainer->>Trainer: CheckInputParameters()
            Trainer->>Instance: classification_train(data_dir, params)
            Note over Python: Запуск обучения в потоке
            Trainer->>Trainer: TrainingStatus = 1
        end
        
        alt TrainingStatus == 1 || 2
            Trainer->>Instance: get_train_status()
            Instance-->>Trainer: TrainingStatus
            Trainer->>Instance: get_epoch(), get_progress()
            Instance-->>Trainer: Epoch, Progress
            Trainer->>Instance: get_train_acc(), get_train_loss()
            Instance-->>Trainer: TrainAcc, TrainLoss
            Trainer->>Instance: get_val_acc(), get_val_loss()
            Instance-->>Trainer: ValAcc, ValLoss
        end
    end
```

### UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> Uninitialized: New()
    Uninitialized --> Defaulted: Default()
    Defaulted --> Configuring: Настройка параметров
    Configuring --> Building: Build()
    Building --> PythonReady: PythonInitialize()
    PythonReady --> Built: APyBuild()
    Built --> Ready: Ready = true
    Ready --> Idle: TrainingStatus = 0
    Idle --> Starting: StartTraining = true
    Starting --> Validating: CheckInputParameters()
    Validating --> Training: Параметры валидны
    Validating --> Idle: Параметры невалидны
    Training --> Running: TrainingStatus = 1
    Running --> Testing: TrainingStatus = 2
    Testing --> Completed: TrainingStatus = 3
    Running --> Error: TrainingStatus = -1
    Testing --> Error: TrainingStatus = -1
    Error --> Idle: Сброс статуса
    Completed --> Idle: Сброс статуса
```

### UML-диаграмма компонентов

```mermaid
graph TB
    subgraph TPyBaseTrainerBase["TPyBaseTrainer"]
        BaseTrainer[TPyBaseTrainer]
    end
    
    subgraph Trainers["Specialized Trainers"]
        ClassifierTrainer[TPyClassifierTrainer]
        DetectorTrainer[TPyDetectorTrainer]
        SegmenterTrainer[TPySegmenterTrainer]
    end
    
    subgraph PythonEnv["Python Environment"]
        ClassifierModule[classifier_interface_tf1]
        DetectorModule[detector_interface]
        SegmenterModule[segmentator_interface]
        TrainingThreads[Training Threads]
    end
    
    BaseTrainer -->|inherits| ClassifierTrainer
    BaseTrainer -->|inherits| DetectorTrainer
    BaseTrainer -->|inherits| SegmenterTrainer
    ClassifierTrainer -->|calls| ClassifierModule
    DetectorTrainer -->|calls| DetectorModule
    SegmenterTrainer -->|calls| SegmenterModule
    Trainers -->|manages| TrainingThreads
```

### TPyClassifierTrainer

#### Свойства

- **`DatasetName`** (string) — имя датасета для обучения
- **`CopySplittedImages`** (bool) — копировать ли разделенные изображения
- **`TestEqualVal`** (bool) — использовать ли одинаковый размер для test и val
- **`ImageSize`** (vector<int>) — размер изображений [width, height, channels]
- **`LearningRate`** (float) — скорость обучения
- **`BatchSizes`** (vector<int>) — размеры батчей [train, val, test]
- **`LayersToBeTrained`** (int) — количество слоев для обучения (0 = default)
- **`Classes`** (vector<string>) — список классов
- **`TrainLoss`**, **`TrainAcc`**, **`ValLoss`**, **`ValAcc`** (float) — метрики обучения

### TPyDetectorTrainer

#### Свойства

- **`DatasetType`** (string) — тип датасета: "xml_main_dirs", "xml_txt_splits", и др.
- **`Config`** (string) — путь к конфигурационному файлу
- **`SavePredicted`** (bool) — сохранять ли предсказания
- **`Visualize`** (int) — режим визуализации (-1 = все, 0 = нет, n = n изображений)
- **`PaintGt`** (bool) — рисовать ли ground truth
- **`LossNames`**, **`TrainLosses`**, **`ValLosses`** — метрики потерь

### TPySegmenterTrainer

#### Свойства

- **`Config`** (string) — путь к конфигурационному файлу
- **`DatasetType`** (string) — тип датасета: "txt_data", "split_data", "not_split_data"
- **`DatasetName`** (string) — имя датасета
- **`InputRes`** (vector<int>) — разрешение входа [height, width, channels]
- **`Classes`** (vector<string>) — список классов
- **`BatchSize`** (int) — размер батча
- **`BatchesInEpoch`** (int) — количество батчей в эпохе
- **`TrainLoss`**, **`TrainAcc`**, **`ValLoss`**, **`ValAcc`** (float) — метрики обучения

### Примеры использования

#### Пример 1: TPyClassifierTrainer

```xml
<Trainer ClassName="PyClassifierTrainer">
    <Property Name="TrainDataDir" Value="datasets/classification" />
    <Property Name="WorkingDir" Value="Results/" />
    <Property Name="ArchitectureName" Value="MobileNet" />
    <Property Name="DatasetName" Value="my_dataset" />
    <Property Name="ImageSize" Value="224,224,3" />
    <Property Name="LearningRate" Value="0.001" />
    <Property Name="BatchSizes" Value="32,16,8" />
    <Property Name="Epochs" Value="50" />
    <Property Name="StartTraining" Value="true" />
</Trainer>
```

#### Пример 2: TPyDetectorTrainer

```xml
<Trainer ClassName="PyDetectorTrainer">
    <Property Name="TrainDataDir" Value="datasets/detection" />
    <Property Name="WorkingDir" Value="Results/" />
    <Property Name="DatasetType" Value="xml_main_dirs" />
    <Property Name="Config" Value="config.cfg" />
    <Property Name="Epochs" Value="100" />
    <Property Name="StartTraining" Value="true" />
</Trainer>
```

### См. также

- [`TPyBaseTrainer`](TPyBaseTrainer.md) — базовый класс тренера
- [`TPyComponent`](TPyComponent.md) — базовый Python-компонент
- [Architecture.md](../Architecture.md) — архитектура библиотеки

---

## EN

### Purpose

**Classes**: 
- `TPyClassifierTrainer` — classifier trainer via Python
- `TPyDetectorTrainer` — detector trainer via Python  
- `TPySegmenterTrainer` — segmentator trainer via Python

All trainers inherit from `TPyBaseTrainer` and provide specialized functionality for training corresponding types of machine learning models.

**Usage:** Training classification, detection and segmentation models via Python

### See also

- [`TPyBaseTrainer`](TPyBaseTrainer.md) — base trainer class
- [`TPyComponent`](TPyComponent.md) — base Python component
- [Architecture.md](../Architecture.md) — library architecture
