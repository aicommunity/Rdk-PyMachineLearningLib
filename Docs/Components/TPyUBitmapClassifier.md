# TPyUBitmapClassifier — классификатор растровых изображений через Python

## RU

### Назначение

**Класс**: `TPyUBitmapClassifier` — классификатор растровых изображений через Python.  
**Регистрация**: `Core/Lib.cpp` → `UploadClass("TPyUBitmapClassifier", "PyUBitmapClassifier")`.  
**Storage-инстансы**: `ClassName = "PyUBitmapClassifier"` в конфигурационных проектах.

`TPyUBitmapClassifier` классифицирует растровые изображения (UBitmap) используя Python модели машинного обучения. Наследуется от `UClassifierBase` и `TPyComponent`, получая функциональность классификатора и Python-интеграции.

**Использование:** Классификация изображений на классы с использованием Python ML библиотек

### UML-диаграмма классов

```mermaid
classDiagram
    UClassifierBase <|-- TPyUBitmapClassifier
    TPyComponent <|-- TPyUBitmapClassifier
    class UClassifierBase {
        +NumClasses : int
        +OutputClasses : MDMatrix~int~
        +OutputConfidences : MDMatrix~double~
        +ClassificationTime : double
    }
    class TPyComponent {
        +PythonScriptFileName : string
        +PythonModuleName : string
        +PythonClassName : string
        +UseFullPath : bool
    }
    class TPyUBitmapClassifier {
        +ImageColorModel : int
        +WeightsPath : string
        +UseWeightsPath : bool
        +UseRelativeWeightsPath : bool
        -ProcessedBmp : UBitmap
        -ProcessedMat : cv::Mat
        +ClassifyBitmap(bmp, confidences, thresh, class_id, is_classified) bool
        +APythonInitialize() bool
        +APyDefault() bool
        +APyBuild() bool
        +APyReset() bool
    }
```

**Иерархия наследования:**
- `UClassifierBase` (Rdk-CRLib) — базовый классификатор
- `TPyComponent` — базовый Python-компонент
- `TPyUBitmapClassifier` — классификатор изображений

### UML-диаграмма последовательности

```mermaid
sequenceDiagram
    participant Storage as UStorage
    participant Classifier as TPyUBitmapClassifier
    participant Python as Python Instance
    participant Input as Input Image
    
    Storage->>Classifier: New()
    Storage->>Classifier: Default()
    Classifier->>Classifier: APyDefault()
    Note over Classifier: PythonModuleName="classifier_interface"<br/>PythonClassName="ClassifierEmbeddingInterface"<br/>NumClasses=4
    Storage->>Classifier: SetWeightsPath("model.h5")
    Storage->>Classifier: SetUseWeightsPath(true)
    Storage->>Classifier: Build()
    Classifier->>Classifier: ABuild()
    Classifier->>Classifier: PythonInitialize()
    Classifier->>Python: Создание экземпляра
    Classifier->>Python: initialize_weights(weights_path)
    Python-->>Classifier: Инициализация успешна
    Classifier-->>Storage: Ready = true
    
    loop Каждый шаг расчета
        Input->>Classifier: UBitmap изображение
        Storage->>Classifier: Calculate()
        Classifier->>Classifier: ClassifyBitmap(bmp, ...)
        Classifier->>Classifier: Конвертация UBitmap → cv::Mat
        Classifier->>Python: classify(ProcessedMat)
        Python-->>Classifier: NumPy array [confidences]
        Classifier->>Classifier: Извлечение confidences из NumPy
        Classifier->>Classifier: Поиск класса с max confidence
        Classifier->>Classifier: Проверка threshold
        Classifier-->>Input: class_id, is_classified, confidences
    end
```

### UML-диаграмма состояний

```mermaid
stateDiagram-v2
    [*] --> Uninitialized: New()
    Uninitialized --> Defaulted: Default()
    Defaulted --> Configuring: SetWeightsPath()<br/>SetUseWeightsPath()
    Configuring --> Building: Build()
    Building --> LoadingPython: PythonInitialize()
    LoadingPython --> InitializingWeights: initialize_weights()
    InitializingWeights --> PythonReady: Веса загружены
    PythonReady --> Built: APyBuild()
    Built --> Ready: Ready = true
    Ready --> Classifying: ClassifyBitmap()
    Classifying --> Processing: Конвертация изображения
    Processing --> CallingPython: Вызов Python classify()
    CallingPython --> ExtractingResults: Извлечение результатов
    ExtractingResults --> Ready: Классификация завершена
    Ready --> Resetting: Reset()
    Resetting --> Ready: Состояния сброшены
```

### UML-диаграмма активности

```mermaid
flowchart TD
    Start([Start ClassifyBitmap]) --> CheckInit{PythonInitialized?}
    CheckInit -->|Нет| EndFail([End: return false])
    CheckInit -->|Да| LockGIL[Захват GIL]
    LockGIL --> GetSize[Получение размеров изображения]
    GetSize --> CopyImage[Копирование изображения в ProcessedBmp]
    CopyImage --> CheckColorModel{ColorModel?}
    CheckColorModel -->|ubmRGB24| ConvertRGB[Конвертация BGR→RGB]
    CheckColorModel -->|ubmY8| UseGrayscale[Использование grayscale]
    CheckColorModel -->|Другое| LogError[LogWarning: Incorrect color model]
    LogError --> EndFail
    ConvertRGB --> CreateMat[Создание cv::Mat]
    UseGrayscale --> CreateMat
    CreateMat --> CallPython[Python: classify ProcessedMat]
    CallPython --> GetResult[Получение NumPy array]
    GetResult --> CheckDims{Размерность == 2?}
    CheckDims -->|Нет| LogError2[LogWarning: Incorrect dimensions]
    LogError2 --> EndFail
    CheckDims -->|Да| CheckSize{result.size == NumClasses?}
    CheckSize -->|Нет| LogError3[LogWarning: Size mismatch]
    LogError3 --> EndFail
    CheckSize -->|Да| ExtractConfidences[Извлечение confidences]
    ExtractConfidences --> FindMax[Поиск max confidence и class_id]
    FindMax --> CheckThreshold{max_conf >= threshold?}
    CheckThreshold -->|Да| SetClassified[is_classified = true]
    CheckThreshold -->|Нет| SetNotClassified[is_classified = false]
    SetClassified --> End([End: return true])
    SetNotClassified --> End
```

### UML-диаграмма компонентов

```mermaid
graph TB
    subgraph UClassifierBase["UClassifierBase (Rdk-CRLib)"]
        BaseClassifier[UClassifierBase]
    end
    
    subgraph TPyComponentBase["TPyComponent"]
        PyComponent[TPyComponent]
    end
    
    subgraph TPyUBitmapClassifier["TPyUBitmapClassifier"]
        Classifier[TPyUBitmapClassifier]
        ImageProcessor[Image Processor]
        PythonBridge[Python Bridge]
    end
    
    subgraph PythonEnv["Python Environment"]
        PythonModule[classifier_interface Module]
        ClassifierClass[ClassifierEmbeddingInterface]
        Model[ML Model]
    end
    
    subgraph OpenCV["OpenCV"]
        MatConverter[cv::Mat Converter]
    end
    
    BaseClassifier -->|inherits| Classifier
    PyComponent -->|inherits| Classifier
    Classifier -->|uses| ImageProcessor
    Classifier -->|uses| PythonBridge
    PythonBridge -->|calls| ClassifierClass
    ClassifierClass -->|uses| Model
    ImageProcessor -->|converts| MatConverter
    MatConverter -->|bridges| PythonModule
```

### Свойства

#### Параметры (ptPubParameter)

- **`ImageColorModel`** (int) — цветовая модель входного изображения:
  - `ubmRGB24=3` — RGB 24-битное
  - `ubmY8=400` — Grayscale 8-битное
  Значение по умолчанию: зависит от базового класса

- **`WeightsPath`** (string) — путь к файлу весов модели (например, .h5 для Keras/TensorFlow). Используется при `UseWeightsPath = true`. Значение по умолчанию: пустая строка

- **`UseWeightsPath`** (bool) — использовать ли путь к весам для инициализации модели. Если `true`, вызывается `initialize_weights(weights_path)` в Python. Значение по умолчанию: `false`

- **`UseRelativeWeightsPath`** (bool) — использовать относительный путь к весам. Если `true`, путь строится относительно `GetCurrentDataDir()`. Значение по умолчанию: `false`

#### Наследуемые свойства

От `UClassifierBase`:
- `NumClasses` (int) — количество классов для классификации
- `OutputClasses` (MDMatrix<int>) — выходные классы
- `OutputConfidences` (MDMatrix<double>) — выходные уверенности (confidences)
- `ClassificationTime` (double) — время классификации в секундах

От `TPyComponent`:
- `PythonScriptFileName` (string) — путь к Python скрипту
- `PythonModuleName` (string) — имя Python модуля (по умолчанию: "classifier_interface")
- `PythonClassName` (string) — имя Python класса (по умолчанию: "ClassifierEmbeddingInterface")
- `UseFullPath` (bool) — использовать абсолютный путь

### Методы

#### Публичные методы

- **`ClassifyBitmap(UBitmap &bmp, MDVector<double> &output_confidences, double conf_thresh, int &class_id, bool &is_classified)`** → `bool` — классифицирует изображение:
  - Конвертирует `UBitmap` в `cv::Mat`
  - Вызывает Python метод `classify(ProcessedMat)`
  - Извлекает confidences из NumPy array
  - Находит класс с максимальной уверенностью
  - Проверяет threshold и устанавливает `is_classified`
  - Возвращает `true` при успехе, `false` при ошибке

#### Защищенные методы

- **`APythonInitialize()`** → `bool` — инициализация Python. Если `UseWeightsPath = true`, вызывает `initialize_weights(weights_path)` в Python. Возвращает `true` при успехе

- **`APyDefault()`** → `bool` — установка значений по умолчанию:
  - `PythonModuleName = "classifier_interface"`
  - `PythonClassName = "ClassifierEmbeddingInterface"`
  - `NumClasses = 4`

- **`APyBuild()`** → `bool` — сборка компонента. Всегда возвращает `true`

- **`APyReset()`** → `bool` — сброс состояния:
  - `ClassificationTime = 0.0`
  - `OutputClasses->Resize(0, 1)`
  - `OutputConfidences->Resize(0, NumClasses)`

### Примеры использования

#### Пример 1: C++ код

```cpp
auto classifier = storage->CreateComponent<TPyUBitmapClassifier>();
classifier->SetPythonScriptFileName("classifier.py");
classifier->SetWeightsPath("model.h5");
classifier->UseWeightsPath = true;
classifier->NumClasses = 10;
classifier->Build();

UBitmap image;
// ... загрузка изображения ...

MDVector<double> confidences;
int class_id;
bool is_classified;
classifier->ClassifyBitmap(image, confidences, 0.5, class_id, is_classified);

if (is_classified) {
    std::cout << "Class: " << class_id << ", Confidence: " << confidences(class_id) << std::endl;
}
```

#### Пример 2: XML конфигурация

```xml
<Classifier ClassName="PyUBitmapClassifier">
    <Property Name="PythonScriptFileName" Value="classifier_interface.py" />
    <Property Name="WeightsPath" Value="models/classifier.h5" />
    <Property Name="UseWeightsPath" Value="true" />
    <Property Name="UseRelativeWeightsPath" Value="true" />
    <Property Name="NumClasses" Value="10" />
</Classifier>
```

### См. также

- [`TPyComponent`](TPyComponent.md) — базовый Python-компонент
- [`TPyClassifierTrainer`](TPyDetectorTrainer.md#tpyclassifiertrainer) — тренер классификаторов
- [Architecture.md](../Architecture.md) — архитектура библиотеки

---

## EN

### Purpose

**Class**: `TPyUBitmapClassifier` — bitmap image classifier via Python.  
**Registration**: `Core/Lib.cpp` → `UploadClass("TPyUBitmapClassifier", "PyUBitmapClassifier")`.  
**Instances**: `ClassName = "PyUBitmapClassifier"` in configuration projects.

`TPyUBitmapClassifier` classifies bitmap images (UBitmap) using Python machine learning models. Inherits from `UClassifierBase` and `TPyComponent`.

**Usage:** Image classification into classes using Python ML libraries

### See also

- [`TPyComponent`](TPyComponent.md) — base Python component
- [`TPyClassifierTrainer`](TPyDetectorTrainer.md#tpyclassifiertrainer) — classifier trainer
- [Architecture.md](../Architecture.md) — library architecture
