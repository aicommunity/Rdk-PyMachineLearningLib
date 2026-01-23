# TPySegmentatorProtobuf — сегментатор с использованием Protobuf

## RU

### Назначение

**Класс**: `TPySegmentatorProtobuf` — сегментатор изображений через Python с использованием Protobuf модели.  
**Регистрация**: `Core/Lib.cpp` → `UploadClass("TPySegmentatorProtobuf", "PySegmentatorProtobuf")`.  
**Storage-инстансы**: `ClassName = "PySegmentatorProtobuf"` в конфигурационных проектах.

`TPySegmentatorProtobuf` выполняет семантическую сегментацию изображений используя Protobuf модель через Python. Наследуется от `TPySegmentator` и `USegmentatorBase`, использует Protobuf файл модели и JSON конфигурацию.

**Использование:** Семантическая сегментация изображений с использованием Protobuf моделей

### UML-диаграмма классов

```mermaid
classDiagram
    TPySegmentator <|-- TPySegmentatorProtobuf
    class TPySegmentator {
        +ImageColorModel : int
        +Inference(bmp, mask) bool
    }
    class TPySegmentatorProtobuf {
        +JSONPath : string
        +ProtobufPath : string
        +APythonInitialize() bool
        +APyDefault2() bool
        +APyBuild2() bool
        +APyReset2() bool
        +Inference(bmp, mask) bool
    }
```

**Иерархия наследования:**
- `USegmentatorBase` (Rdk-CRLib) — базовый сегментатор
- `TPyComponent` — базовый Python-компонент
- `TPySegmentator` — базовый сегментатор
- `TPySegmentatorProtobuf` — Protobuf сегментатор

### UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Segmentator as TPySegmentatorProtobuf
    participant Python as Python Instance
    participant Input as Input Image
    
    Storage->>Segmentator: New()
    Storage->>Segmentator: Default()
    Segmentator->>Segmentator: APyDefault()
    Segmentator->>Segmentator: APyDefault2()
    Note over Segmentator: PythonModuleName="segmentator_interface"<br/>PythonClassName="SegmentatorEmbeddingInterface"
    Storage->>Segmentator: SetProtobufPath("model.pb")
    Storage->>Segmentator: SetJSONPath("config.json")
    Storage->>Segmentator: Build()
    Segmentator->>Segmentator: ABuild()
    Segmentator->>Segmentator: PythonInitialize()
    Segmentator->>Python: Создание экземпляра
    Segmentator->>Python: initialize_config(protobuf_path, json_path)
    Python-->>Segmentator: Инициализация успешна
    Segmentator->>Segmentator: APyBuild2()
    Segmentator-->>Storage: Ready = true
    
    loop Каждый шаг расчета
        Input->>Segmentator: UBitmap изображение
        Storage->>Segmentator: Calculate()
        Segmentator->>Segmentator: Inference(bmp, mask)
        Segmentator->>Python: inference(bmp)
        Python-->>Segmentator: NumPy array [mask]
        Segmentator->>Segmentator: Конвертация NumPy → cv::Mat
        Segmentator->>Segmentator: Конвертация cv::Mat → UBitmap
        Segmentator-->>Input: mask
    end
```

### UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> Uninitialized: New()
    Uninitialized --> Defaulted: Default()
    Defaulted --> Configuring: SetProtobufPath()<br/>SetJSONPath()
    Configuring --> Building: Build()
    Building --> LoadingPython: PythonInitialize()
    LoadingPython --> InitializingProtobuf: initialize_config(protobuf, json)
    InitializingProtobuf --> PythonReady: Конфигурация загружена
    PythonReady --> Built: APyBuild2()
    Built --> Ready: Ready = true
    Ready --> Segmenting: Inference()
    Segmenting --> Processing: Обработка изображения
    Processing --> CallingPython: Вызов Python inference()
    CallingPython --> Converting: Конвертация результатов
    Converting --> Ready: Сегментация завершена
    Ready --> Resetting: Reset()
    Resetting --> Ready: Состояния сброшены
```

### UML-диаграмма активности

```mermaid
flowchart TD
    Start([Start Inference]) --> CheckInit{PythonInitialized?}
    CheckInit -->|Нет| EndFail([End: return false])
    CheckInit -->|Да| LockGIL[Захват GIL]
    LockGIL --> CallPython[Python: inference bmp]
    CallPython --> GetResult[Получение NumPy array]
    GetResult --> ConvertMat[Конвертация NumPy → cv::Mat]
    ConvertMat --> CheckType{Тип Mat?}
    CheckType -->|CV_8UC3| ConvertRGB[Конвертация RGB]
    CheckType -->|CV_8U| UseGrayscale[Использование grayscale]
    CheckType -->|Другое| LogError[LogWarning: Incorrect type]
    LogError --> EndFail
    ConvertRGB --> CreateBitmap[Создание UBitmap]
    UseGrayscale --> CreateBitmap
    CreateBitmap --> End([End: return true])
```

### UML-диаграмма компонентов

```mermaid
graph TB
    subgraph TPySegmentatorBase["TPySegmentator"]
        BaseSegmentator[TPySegmentator]
    end
    
    subgraph TPySegmentatorProtobuf["TPySegmentatorProtobuf"]
        ProtobufSegmentator[TPySegmentatorProtobuf]
        ProtobufModel[Protobuf Model]
        JSONConfig[JSON Configuration]
    end
    
    subgraph PythonEnv["Python Environment"]
        SegmentatorModule[segmentator_interface Module]
        SegmentatorClass[SegmentatorEmbeddingInterface]
        ProtobufLoader[Protobuf Loader]
    end
    
    subgraph OpenCV["OpenCV"]
        MatConverter[cv::Mat Converter]
    end
    
    BaseSegmentator -->|inherits| ProtobufSegmentator
    ProtobufSegmentator -->|uses| ProtobufModel
    ProtobufSegmentator -->|uses| JSONConfig
    ProtobufSegmentator -->|calls| SegmentatorClass
    SegmentatorClass -->|uses| ProtobufLoader
    ProtobufLoader -->|loads| ProtobufModel
    ProtobufSegmentator -->|converts| MatConverter
```

### Свойства

#### Параметры (ptPubParameter)

- **`ProtobufPath`** (string) — путь к Protobuf файлу модели (.pb). Используется при инициализации через `initialize_config(protobuf_path, json_path)`. Значение по умолчанию: пустая строка

- **`JSONPath`** (string) — путь к JSON файлу с конфигурацией модели. Используется при инициализации. Значение по умолчанию: пустая строка

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

#### Защищенные методы

- **`APythonInitialize()`** → `bool` — инициализация Python:
  - Вызывает `initialize_config(protobuf_path, json_path)` в Python
  - Возвращает `true` при успехе, `false` при ошибке

- **`APyDefault2()`** → `bool` — дополнительная инициализация. Всегда возвращает `true`

- **`APyBuild2()`** → `bool` — дополнительная сборка. Всегда возвращает `true`

- **`APyReset2()`** → `bool` — дополнительный сброс. Всегда возвращает `true`

- **`Inference(UBitmap &bmp, UBitmap &mask)`** → `bool` — выполняет сегментацию изображения:
  - Проверяет инициализацию Python
  - Вызывает Python метод `inference(bmp)`
  - Получает NumPy array с маской сегментации
  - Конвертирует NumPy → cv::Mat через `fromNDArrayToMat()`
  - Определяет тип Mat (RGB или Grayscale)
  - Конвертирует cv::Mat → UBitmap
  - Для RGB применяет `SwapRGBChannels()`
  - Возвращает `true` при успехе, `false` при ошибке

### Примеры использования

#### Пример 1: C++ код

```cpp
auto segmentator = storage->CreateComponent<TPySegmentatorProtobuf>();
segmentator->SetPythonScriptFileName("segmentator_interface.py");
segmentator->ProtobufPath = "model.pb";
segmentator->JSONPath = "config.json";
segmentator->Build();

UBitmap image;
UBitmap mask;
// ... загрузка изображения ...

segmentator->Inference(image, mask);
// mask содержит результат сегментации
```

#### Пример 2: XML конфигурация

```xml
<Segmentator ClassName="PySegmentatorProtobuf">
    <Property Name="PythonScriptFileName" Value="segmentator_interface.py" />
    <Property Name="ProtobufPath" Value="models/segmentator.pb" />
    <Property Name="JSONPath" Value="models/config.json" />
    <Property Name="UseFullPath" Value="false" />
</Segmentator>
```

### Особенности работы

1. **Protobuf формат**: Использует Protobuf формат для модели, что обеспечивает компактность и эффективность

2. **JSON конфигурация**: Требует JSON файл с конфигурацией модели для инициализации

3. **Конвертация форматов**: Автоматическая конвертация между NumPy arrays, cv::Mat и UBitmap

4. **Поддержка форматов**: Поддерживает как RGB, так и Grayscale изображения

5. **Обработка RGB**: Для RGB изображений автоматически применяется `SwapRGBChannels()` для корректного отображения

### См. также

- [`TPySegmentator`](TPySegmentator.md) — базовый сегментатор
- [`TPySegmentatorUNet`](TPySegmentatorUNet.md) — U-Net сегментатор
- [`TPySegmenterTrainer`](TPySegmenterTrainer.md) — тренер сегментаторов
- [Architecture.md](../Architecture.md) — архитектура библиотеки

---

## EN

### Purpose

**Class**: `TPySegmentatorProtobuf` — image segmentator via Python using Protobuf model.  
**Registration**: `Core/Lib.cpp` → `UploadClass("TPySegmentatorProtobuf", "PySegmentatorProtobuf")`.  
**Instances**: `ClassName = "PySegmentatorProtobuf"` in configuration projects.

`TPySegmentatorProtobuf` performs semantic image segmentation using Protobuf model via Python. Inherits from `TPySegmentator` and `USegmentatorBase`.

**Usage:** Semantic image segmentation using Protobuf models

### See also

- [`TPySegmentator`](TPySegmentator.md) — base segmentator
- [`TPySegmentatorUNet`](TPySegmentatorUNet.md) — U-Net segmentator
- [`TPySegmenterTrainer`](TPySegmenterTrainer.md) — segmentator trainer
- [Architecture.md](../Architecture.md) — library architecture
