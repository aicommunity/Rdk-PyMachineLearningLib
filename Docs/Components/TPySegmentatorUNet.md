# TPySegmentatorUNet — U-Net сегментатор через Python

## RU

### Назначение

**Класс**: `TPySegmentatorUNet` — U-Net сегментатор изображений через Python.  
**Регистрация**: `Core/Lib.cpp` → `UploadClass("TPySegmentatorUNet", "TPySegmentatorUNet")`.  
**Storage-инстансы**: `ClassName = "TPySegmentatorUNet"` в конфигурационных проектах.

`TPySegmentatorUNet` выполняет семантическую сегментацию изображений используя U-Net архитектуру через Python. Наследуется от `TPySegmentator` и `USegmentatorBase`, получая функциональность сегментатора и Python-интеграции.

**Использование:** Семантическая сегментация изображений с использованием U-Net моделей

### UML-диаграмма классов

```mermaid
classDiagram
    USegmentatorBase <|-- TPySegmentator
    TPyComponent <|-- TPySegmentator
    TPySegmentator <|-- TPySegmentatorUNet
    TPySegmentator <|-- TPySegmentatorProtobuf
    class USegmentatorBase {
        +InputImage : UBitmap
        +OutputMask : UBitmap
    }
    class TPyComponent {
        +PythonScriptFileName : string
        +PythonModuleName : string
        +PythonClassName : string
    }
    class TPySegmentator {
        +ImageColorModel : int
        +Inference(bmp, mask) bool
        +APyDefault() bool
        +APyDefault2() bool
        +APyBuild() bool
        +APyBuild2() bool
        +APyReset() bool
        +APyReset2() bool
    }
    class TPySegmentatorUNet {
        +WeightsPath : string
        +RespondThreshold : double
        +APythonInitialize() bool
        +APyDefault2() bool
        +APyBuild2() bool
        +APyReset2() bool
    }
```

**Иерархия наследования:**
- `USegmentatorBase` (Rdk-CRLib) — базовый сегментатор
- `TPyComponent` — базовый Python-компонент
- `TPySegmentator` — базовый сегментатор
- `TPySegmentatorUNet` — U-Net сегментатор

### UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Segmentator as TPySegmentatorUNet
    participant Python as Python Instance
    participant Input as Input Image
    
    Storage->>Segmentator: New()
    Storage->>Segmentator: Default()
    Segmentator->>Segmentator: APyDefault()
    Note over Segmentator: PythonModuleName="segmentator_interface"<br/>PythonClassName="SegmentatorEmbeddingInterface"<br/>RespondThreshold=64.0
    Segmentator->>Segmentator: APyDefault2()
    Storage->>Segmentator: SetWeightsPath("unet.h5")
    Storage->>Segmentator: Build()
    Segmentator->>Segmentator: ABuild()
    Segmentator->>Segmentator: PythonInitialize()
    Segmentator->>Python: Создание экземпляра
    Segmentator->>Python: initialize_config(weights_path)
    Python-->>Segmentator: Инициализация успешна
    Segmentator->>Segmentator: APyBuild()
    Segmentator->>Segmentator: APyBuild2()
    Segmentator-->>Storage: Ready = true
    
    loop Каждый шаг расчета
        Input->>Segmentator: UBitmap изображение
        Storage->>Segmentator: Calculate()
        Segmentator->>Segmentator: Inference(bmp, mask)
        Segmentator->>Python: inference(bmp)
        Python-->>Segmentator: NumPy array [mask]
        Segmentator->>Segmentator: Конвертация NumPy → cv::Mat
        Segmentator->>Segmentator: Threshold с RespondThreshold
        Segmentator->>Segmentator: Конвертация cv::Mat → UBitmap
        Segmentator-->>Input: mask
    end
```

### UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> Uninitialized: New()
    Uninitialized --> Defaulted: Default()
    Defaulted --> Configuring: SetWeightsPath()<br/>SetRespondThreshold()
    Configuring --> Building: Build()
    Building --> LoadingPython: PythonInitialize()
    LoadingPython --> InitializingConfig: initialize_config()
    InitializingConfig --> PythonReady: Конфигурация загружена
    PythonReady --> Built: APyBuild()
    Built --> Ready: Ready = true
    Ready --> Segmenting: Inference()
    Segmenting --> Processing: Обработка изображения
    Processing --> CallingPython: Вызов Python inference()
    CallingPython --> Converting: Конвертация результатов
    Converting --> Thresholding: Применение threshold
    Thresholding --> Ready: Сегментация завершена
    Ready --> Resetting: Reset()
    Resetting --> Ready: Состояния сброшены
```

### UML-диаграмма активности

```mermaid
flowchart TD
    Start([Start Inference]) --> LockGIL[Захват GIL]
    LockGIL --> CallPython[Python: inference bmp]
    CallPython --> GetResult[Получение NumPy array]
    GetResult --> ConvertMat[Конвертация NumPy → cv::Mat]
    ConvertMat --> ApplyThreshold[Threshold с RespondThreshold]
    ApplyThreshold --> CheckType{Тип Mat?}
    CheckType -->|CV_8UC3| ConvertRGB[Конвертация RGB]
    CheckType -->|CV_8U| UseGrayscale[Использование grayscale]
    CheckType -->|Другое| LogError[LogWarning: Incorrect type]
    LogError --> EndFail([End: return false])
    ConvertRGB --> CreateBitmap[Создание UBitmap]
    UseGrayscale --> CreateBitmap
    CreateBitmap --> End([End: return true])
```

### UML-диаграмма компонентов

```mermaid
graph TB
    subgraph USegmentatorBase["USegmentatorBase (Rdk-CRLib)"]
        BaseSegmentator[USegmentatorBase]
    end
    
    subgraph TPyComponentBase["TPyComponent"]
        PyComponent[TPyComponent]
    end
    
    subgraph TPySegmentatorBase["TPySegmentator"]
        BaseSegmentatorClass[TPySegmentator]
    end
    
    subgraph TPySegmentatorUNet["TPySegmentatorUNet"]
        UNetSegmentator[TPySegmentatorUNet]
        ImageProcessor[Image Processor]
        ThresholdProcessor[Threshold Processor]
    end
    
    subgraph PythonEnv["Python Environment"]
        SegmentatorModule[segmentator_interface Module]
        SegmentatorClass[SegmentatorEmbeddingInterface]
        UNetModel[U-Net Model]
    end
    
    subgraph OpenCV["OpenCV"]
        MatConverter[cv::Mat Converter]
        ThresholdOp[Threshold Operations]
    end
    
    BaseSegmentator -->|inherits| BaseSegmentatorClass
    PyComponent -->|inherits| BaseSegmentatorClass
    BaseSegmentatorClass -->|base for| UNetSegmentator
    UNetSegmentator -->|uses| ImageProcessor
    UNetSegmentator -->|uses| ThresholdProcessor
    ImageProcessor -->|converts| MatConverter
    ThresholdProcessor -->|uses| ThresholdOp
    UNetSegmentator -->|calls| SegmentatorClass
    SegmentatorClass -->|uses| UNetModel
```

### Свойства

#### Параметры (ptPubParameter)

- **`WeightsPath`** (string) — путь к файлу весов U-Net модели (например, .h5 для Keras/TensorFlow). Используется при инициализации через `initialize_config(weights_path)`. Значение по умолчанию: пустая строка

- **`RespondThreshold`** (double) — порог для бинаризации маски сегментации. Применяется к результату через `cv::threshold()`. Значение по умолчанию: 64.0

#### Наследуемые свойства

От `USegmentatorBase`:
- `InputImage` (UBitmap) — входное изображение
- `OutputMask` (UBitmap) — выходная маска сегментации

От `TPySegmentator`:
- `ImageColorModel` (int) — цветовая модель входного изображения

От `TPyComponent`:
- `PythonScriptFileName` (string) — путь к Python скрипту
- `PythonModuleName` (string) — имя Python модуля (по умолчанию: "segmentator_interface")
- `PythonClassName` (string) — имя Python класса (по умолчанию: "SegmentatorEmbeddingInterface")

### Методы

#### Публичные методы

- **`Inference(UBitmap &bmp, UBitmap &mask)`** → `bool` — выполняет сегментацию изображения:
  - Вызывает Python метод `inference(bmp)`
  - Получает NumPy array с маской сегментации
  - Конвертирует NumPy → cv::Mat
  - Применяет threshold с `RespondThreshold`
  - Конвертирует cv::Mat → UBitmap
  - Возвращает `true` при успехе

#### Защищенные методы

- **`APythonInitialize()`** → `bool` — инициализация Python. Вызывает `initialize_config(weights_path)` в Python. Возвращает `true` при успехе

- **`APyDefault2()`** → `bool` — установка значений по умолчанию:
  - `RespondThreshold = 64.0`

- **`APyBuild2()`** → `bool` — сборка компонента. Всегда возвращает `true`

- **`APyReset2()`** → `bool` — сброс состояния. Всегда возвращает `true`

### Примеры использования

#### Пример 1: C++ код

```cpp
auto segmentator = storage->CreateComponent<TPySegmentatorUNet>();
segmentator->SetPythonScriptFileName("segmentator_interface.py");
segmentator->SetWeightsPath("unet_model.h5");
segmentator->RespondThreshold = 128.0;
segmentator->Build();

UBitmap image;
UBitmap mask;
// ... загрузка изображения ...

segmentator->Inference(image, mask);
// mask содержит результат сегментации
```

#### Пример 2: XML конфигурация

```xml
<Segmentator ClassName="TPySegmentatorUNet">
    <Property Name="PythonScriptFileName" Value="segmentator_interface.py" />
    <Property Name="WeightsPath" Value="models/unet.h5" />
    <Property Name="UseRelativeWeightsPath" Value="true" />
    <Property Name="RespondThreshold" Value="128.0" />
</Segmentator>
```

### Особенности работы

1. **Threshold обработка**: Результат сегментации из Python автоматически проходит через бинаризацию с порогом `RespondThreshold`

2. **Конвертация форматов**: Автоматическая конвертация между NumPy arrays, cv::Mat и UBitmap

3. **Поддержка форматов**: Поддерживает как RGB, так и Grayscale изображения

### Специализированные сегментаторы

#### TPySegmentatorProtobuf

Сегментатор с использованием Protobuf модели.

**Дополнительные свойства:**
- `JSONPath` (string) — путь к JSON файлу с конфигурацией
- `ProtobufPath` (string) — путь к Protobuf файлу модели

**Инициализация:** Вызывается `initialize_config(protobuf_path, json_path)`

**Особенности:**
- Использует Protobuf формат для модели
- Требует JSON конфигурационный файл

### См. также

- [`TPyComponent`](TPyComponent.md) — базовый Python-компонент
- [`TPySegmentator`](TPySegmentatorUNet.md#tpysegmentator) — базовый сегментатор
- [`TPySegmenterTrainer`](TPyDetectorTrainer.md#tpysegmentertrainer) — тренер сегментаторов
- [Architecture.md](../Architecture.md) — архитектура библиотеки

---

## EN

### Purpose

**Class**: `TPySegmentatorUNet` — U-Net image segmentator via Python.  
**Registration**: `Core/Lib.cpp` → `UploadClass("TPySegmentatorUNet", "TPySegmentatorUNet")`.  
**Instances**: `ClassName = "TPySegmentatorUNet"` in configuration projects.

`TPySegmentatorUNet` performs semantic image segmentation using U-Net architecture via Python. Inherits from `TPySegmentator` and `USegmentatorBase`.

**Usage:** Semantic image segmentation using U-Net models

### See also

- [`TPyComponent`](TPyComponent.md) — base Python component
- [`TPySegmenterTrainer`](TPyDetectorTrainer.md#tpysegmentertrainer) — segmentator trainer
- [Architecture.md](../Architecture.md) — library architecture
