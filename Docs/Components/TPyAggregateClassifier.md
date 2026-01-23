# TPyAggregateClassifier — агрегатный классификатор (ансамбль)

## RU

### Назначение

**Класс**: `TPyAggregateClassifier` — агрегатный классификатор для классификации объектов в заданных областях изображения через Python.  
**Регистрация**: `Core/Lib.cpp` → `UploadClass("TPyAggregateClassifier", "PyAggregateClassifier")`.  
**Storage-инстансы**: `ClassName = "PyAggregateClassifier"` в конфигурационных проектах.

`TPyAggregateClassifier` классифицирует объекты в заданных прямоугольных областях (агрегатах) изображения. Наследуется от `UNet` и использует Python для классификации каждого агрегата. Результаты классификации сохраняются в матрице детекций.

**Использование:** Классификация объектов в заданных областях изображения (постобработка детекции)

### UML-диаграмма классов

```mermaid
classDiagram
    UNet <|-- TPyAggregateClassifier
    class UNet {
        +ADefault() bool
        +ABuild() bool
        +AReset() bool
        +ACalculate() bool
    }
    class TPyAggregateClassifier {
        +InputImage : UBitmap
        +PythonScriptFileName : string
        +AggrRectsMatrix : MDMatrix~int~
        +AggrIdMatrix : MDMatrix~int~
        +Detections : MDMatrix~int~
        +DebugImage : UBitmap
        -IntegrationInterface : boost::python::object*
        -IntegrationInterfaceInstance : boost::python::object*
        -Initialized : bool
        -Graph : UGraphics
        -Canvas : UBitmap
        +New() TPyAggregateClassifier*
        +Initialize() bool
        +AInit() void
        +AUnInit() void
        +ADefault() bool
        +ABuild() bool
        +AReset() bool
        +ACalculate() bool
    }
```

**Иерархия наследования:**
- `UNet` (Rdk) — базовая сеть компонентов
- `TPyAggregateClassifier` — агрегатный классификатор

### UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Classifier as TPyAggregateClassifier
    participant Python as Python Instance
    participant Input as Input Image
    
    Storage->>Classifier: New()
    Storage->>Classifier: Default()
    Classifier->>Classifier: ADefault()
    Note over Classifier: Initialized=false<br/>PythonScriptFileName=""
    Storage->>Classifier: SetPythonScriptFileName("classifier.py")
    Storage->>Classifier: SetAggrRectsMatrix(...)
    Storage->>Classifier: SetAggrIdMatrix(...)
    Storage->>Classifier: Build()
    Classifier->>Classifier: ABuild()
    Classifier->>Classifier: Initialize()
    Classifier->>Python: Создание экземпляра
    Classifier->>Classifier: Initialized = true
    Classifier-->>Storage: Ready = true
    
    loop Каждый шаг расчета
        Input->>Classifier: InputImage (UBitmap)
        Input->>Classifier: AggrRectsMatrix [x1,y1,x2,y2]
        Input->>Classifier: AggrIdMatrix [id]
        Storage->>Classifier: Calculate()
        Classifier->>Classifier: ACalculate()
        loop Для каждого агрегата
            Classifier->>Classifier: Извлечение области изображения
            Classifier->>Python: classify(obj_rect)
            Python-->>Classifier: class_id
            Classifier->>Classifier: Запись в Detections
            Classifier->>Classifier: Визуализация на DebugImage
        end
        Classifier-->>Input: Detections [id, x1, y1, x2, y2, class]
    end
```

### UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> Uninitialized: New()
    Uninitialized --> Defaulted: Default()
    Defaulted --> Configuring: SetPythonScriptFileName()<br/>SetAggrRectsMatrix()
    Configuring --> Building: Build()
    Building --> Initializing: Initialize()
    Initializing --> LoadingPython: Загрузка Python модуля
    LoadingPython --> PythonReady: Python готов
    PythonReady --> Built: Initialized = true
    Built --> Ready: Ready = true
    Ready --> Classifying: ACalculate()
    Classifying --> Processing: Обработка агрегатов
    Processing --> ExtractingRegions: Извлечение областей
    ExtractingRegions --> CallingPython: Вызов Python classify()
    CallingPython --> StoringResults: Сохранение результатов
    StoringResults --> Visualizing: Визуализация на DebugImage
    Visualizing --> Ready: Классификация завершена
    Ready --> Resetting: Reset()
    Resetting --> Ready: Состояния сброшены
```

### UML-диаграмма активности

```mermaid
flowchart TD
    Start([Start ACalculate]) --> CheckInit{Initialized?}
    CheckInit -->|Нет| End([End: return true])
    CheckInit -->|Да| CheckInput{InputImage<br/>IsConnected?}
    CheckInput -->|Нет| End
    CheckInput -->|Да| CheckColorModel{ColorModel == ubmY8?}
    CheckColorModel -->|Нет| LogError[LogWarning: Incorrect color model]
    LogError --> End
    CheckColorModel -->|Да| ConvertImage[Конвертация в DebugImage RGB24]
    ConvertImage --> SetCanvas[SetCanvas DebugImage]
    SetCanvas --> ResizeDetections[Resize Detections 0x6]
    ResizeDetections --> CheckRows{Rows AggrRects ==<br/>Rows AggrId?}
    CheckRows -->|Нет| LogError2[LogWarning: Rows mismatch]
    LogError2 --> End
    CheckRows -->|Да| ResizeDetections2[Resize Detections rows x 6]
    ResizeDetections2 --> LoopAggregates[Цикл по агрегатам]
    LoopAggregates --> GetRect[Получение rect из AggrRectsMatrix]
    GetRect --> GetId[Получение id из AggrIdMatrix]
    GetId --> CalcSize[Вычисление width, height]
    CalcSize --> CheckSize{width > 3 &&<br/>height > 3?}
    CheckSize -->|Нет| NextAggregate[Следующий агрегат]
    CheckSize -->|Да| ExtractRegion[Извлечение области изображения]
    ExtractRegion --> CallPython[Python: classify obj_rect]
    CallPython --> GetClass[Получение class_id]
    GetClass --> StoreDetection[Запись в Detections]
    StoreDetection --> Visualize[Визуализация на DebugImage]
    Visualize --> NextAggregate
    NextAggregate --> MoreAggregates{Еще агрегаты?}
    MoreAggregates -->|Да| LoopAggregates
    MoreAggregates -->|Нет| End
```

### UML-диаграмма компонентов

```mermaid
graph TB
    subgraph UNetBase["UNet (Rdk)"]
        UNet[UNet]
    end
    
    subgraph TPyAggregateClassifier["TPyAggregateClassifier"]
        AggregateClassifier[TPyAggregateClassifier]
        RegionExtractor[Region Extractor]
        PythonBridge[Python Bridge]
        Visualizer[Visualizer]
    end
    
    subgraph PythonEnv["Python Environment"]
        ClassifierModule[test_class Module]
        ClassifierClass[ClassifierEmbeddingInterface]
        ClassifierModel[Classifier Model]
    end
    
    subgraph Inputs["Inputs"]
        InputImage[InputImage UBitmap]
        AggrRects[AggrRectsMatrix]
        AggrIds[AggrIdMatrix]
    end
    
    subgraph Outputs["Outputs"]
        Detections[Detections Matrix]
        DebugImage[DebugImage UBitmap]
    end
    
    UNet -->|inherits| AggregateClassifier
    AggregateClassifier -->|uses| RegionExtractor
    AggregateClassifier -->|uses| PythonBridge
    AggregateClassifier -->|uses| Visualizer
    PythonBridge -->|calls| ClassifierClass
    ClassifierClass -->|uses| ClassifierModel
    Inputs -->|provides| AggregateClassifier
    AggregateClassifier -->|produces| Outputs
```

### Свойства

#### Параметры (ptPubParameter)

- **`InputImage`** (UBitmap) — входное изображение для классификации. Должно быть подключено через `IsConnected()`. Значение по умолчанию: не подключено

- **`PythonScriptFileName`** (string) — путь к Python скрипту с классификатором. Используется при инициализации. Значение по умолчанию: пустая строка

- **`AggrRectsMatrix`** (MDMatrix<int>, ptPubInput) — матрица прямоугольников агрегатов [Left, Top, Right, Bottom]. Каждая строка содержит координаты одного агрегата. Значение по умолчанию: пустая матрица

- **`AggrIdMatrix`** (MDMatrix<int>, ptPubInput) — матрица идентификаторов агрегатов [AggrID]. Соответствует строкам `AggrRectsMatrix`. Значение по умолчанию: пустая матрица

- **`DebugImage`** (UBitmap, ptPubParameter) — изображение для визуализации результатов классификации. На нем рисуются прямоугольники с цветами в зависимости от класса. Значение по умолчанию: пустое изображение

#### Состояние

- **`Detections`** (MDMatrix<int>) — выходная матрица детекций [AggrID, x1, y1, x2, y2, class_id]. Создается в `ACalculate()`. Значение по умолчанию: пустая матрица

#### Защищенные свойства

- **`IntegrationInterface`** (boost::python::object*) — указатель на объект Python класса
- **`IntegrationInterfaceInstance`** (boost::python::object*) — указатель на экземпляр Python класса
- **`Initialized`** (bool) — флаг инициализации Python модуля
- **`Graph`** (UGraphics) — графический контекст для визуализации
- **`Canvas`** (UBitmap) — холст для визуализации

### Методы

#### Публичные методы

- **`New()`** → `TPyAggregateClassifier*` — создает новый экземпляр класса

- **`Initialize()`** → `bool` — инициализирует Python модуль:
  - Проверяет инициализацию потоков Python
  - Импортирует главный модуль Python
  - Загружает пользовательский модуль из файла
  - Создает экземпляр класса `ClassifierEmbeddingInterface`
  - Устанавливает `Initialized = true`
  - Возвращает `true` при успехе

#### Защищенные методы жизненного цикла

- **`AInit()`** → `void` — инициализация компонента. В текущей реализации пустая

- **`AUnInit()`** → `void` — деинициализация компонента. В текущей реализации пустая

- **`ADefault()`** → `bool` — установка значений по умолчанию:
  - `Initialized = false`
  - Возвращает `true`

- **`ABuild()`** → `bool` — сборка компонента:
  - Если `IsInit()`, вызывает `Initialize()`
  - Возвращает `true`

- **`AReset()`** → `bool` — сброс состояния:
  - Если не инициализирован, вызывает `Initialize()`
  - Возвращает `true`

- **`ACalculate()`** → `bool` — выполнение расчета:
  - Проверяет подключение `InputImage`
  - Проверяет цветовую модель (должна быть `ubmY8`)
  - Конвертирует изображение в `DebugImage` (RGB24)
  - Проверяет соответствие размеров `AggrRectsMatrix` и `AggrIdMatrix`
  - Для каждого агрегата:
    - Извлекает область изображения
    - Вызывает Python `classify(obj_rect)`
    - Сохраняет результат в `Detections`
    - Визуализирует результат на `DebugImage`
  - Возвращает `true`

### Примеры использования

#### Пример 1: C++ код

```cpp
auto classifier = storage->CreateComponent<TPyAggregateClassifier>();
classifier->SetPythonScriptFileName("classifier.py");
classifier->Build();

// Подключение входов
UBitmap inputImage;
MDMatrix<int> aggrRects;  // [x1, y1, x2, y2]
MDMatrix<int> aggrIds;    // [id]

// Заполнение данных
aggrRects.Resize(2, 4);
aggrRects(0, 0) = 10; aggrRects(0, 1) = 20;
aggrRects(0, 2) = 50; aggrRects(0, 3) = 60;
aggrRects(1, 0) = 100; aggrRects(1, 1) = 120;
aggrRects(1, 2) = 150; aggrRects(1, 3) = 160;

aggrIds.Resize(2, 1);
aggrIds(0, 0) = 1;
aggrIds(1, 0) = 2;

classifier->InputImage.Connect(&inputImage);
classifier->AggrRectsMatrix.Connect(&aggrRects);
classifier->AggrIdMatrix.Connect(&aggrIds);

// Расчет
classifier->Reset();
classifier->Calculate();

// Получение результатов
MDMatrix<int>& detections = *classifier->Detections;
for (int i = 0; i < detections.GetRows(); i++) {
    std::cout << "Aggregate " << detections(i, 0) 
              << ": class=" << detections(i, 5) << std::endl;
}
```

#### Пример 2: XML конфигурация

```xml
<AggregateClassifier ClassName="PyAggregateClassifier">
    <Property Name="PythonScriptFileName" Value="classifier_interface.py" />
    <Property Name="InputImage" Connect="SourceComponent.OutputImage" />
    <Property Name="AggrRectsMatrix" Connect="DetectorComponent.OutputRects" />
    <Property Name="AggrIdMatrix" Connect="DetectorComponent.OutputIds" />
</AggregateClassifier>
```

### Особенности работы

1. **Обработка агрегатов**: Компонент обрабатывает каждый агрегат независимо, вызывая Python классификатор для каждой области

2. **Визуализация**: Результаты визуализируются на `DebugImage` с цветовым кодированием классов:
   - Класс 0 — синий (0x0000FF)
   - Класс 1 — зеленый (0x00FF00)

3. **Фильтрация**: Агрегаты с размерами меньше 3x3 пикселей пропускаются

4. **Формат выходных данных**: `Detections` содержит [AggrID, x1, y1, x2, y2, class_id] для каждого агрегата

### Связи с другими компонентами

- **Использует**: Python модуль `test_class` с классом `ClassifierEmbeddingInterface`
- **Типичное использование**: В паре с детекторами объектов для классификации обнаруженных областей

### См. также

- [`TPyUBitmapClassifier`](TPyUBitmapClassifier.md) — классификатор изображений
- [`TPyObjectDetector`](TPyObjectDetector.md) — детектор объектов
- [Architecture.md](../Architecture.md) — архитектура библиотеки

---

## EN

### Purpose

**Class**: `TPyAggregateClassifier` — aggregate classifier for classifying objects in specified image regions via Python.  
**Registration**: `Core/Lib.cpp` → `UploadClass("TPyAggregateClassifier", "PyAggregateClassifier")`.  
**Instances**: `ClassName = "PyAggregateClassifier"` in configuration projects.

`TPyAggregateClassifier` classifies objects in specified rectangular regions (aggregates) of an image. Inherits from `UNet` and uses Python to classify each aggregate.

**Usage:** Classifying objects in specified image regions (post-processing of detection)

### See also

- [`TPyUBitmapClassifier`](TPyUBitmapClassifier.md) — image classifier
- [`TPyObjectDetector`](TPyObjectDetector.md) — object detector
- [Architecture.md](../Architecture.md) — library architecture
