# Обзор API Rdk-PyMachineLearningLib

## RU

### Основные классы

#### PyMachineLearningLib

Главный класс библиотеки, наследник `ULibrary`.

#### TPyComponent

Базовый компонент для работы с Python.

**Основные свойства:**
- `PythonScript` - путь к Python скрипту
- `PythonEnvironment` - окружение Python

#### TPyUBitmapClassifier

Классификатор изображений через Python.

**Основные свойства:**
- `ModelPath` - путь к модели
- `InputImage` - входное изображение
- `ClassLabel` - метка класса
- `Confidence` - уверенность

#### TPyObjectDetectorYolo

YOLO детектор объектов.

**Основные свойства:**
- `ModelPath` - путь к модели YOLO
- `ConfigPath` - путь к конфигурации
- `InputImage` - входное изображение
- `DetectedObjects` - обнаруженные объекты

#### TPySegmentatorUNet

Сегментатор UNet.

**Основные свойства:**
- `ModelPath` - путь к модели UNet
- `InputImage` - входное изображение
- `SegmentationMask` - маска сегментации

#### TPythonIntegration

Интеграция с Python.

**Основные методы:**
- `CallPythonFunction()` - вызов Python функции
- `LoadPythonModule()` - загрузка Python модуля

### См. также

- Исходный код: `Libraries/Rdk-PyMachineLearningLib/Core/`

---

## EN

### Main Classes

#### PyMachineLearningLib

Main library class, inherits from `ULibrary`.

#### TPyComponent

Base component for Python operations.

**Main Properties:**
- `PythonScript` - path to Python script
- `PythonEnvironment` - Python environment

#### TPyUBitmapClassifier

Image classifier via Python.

**Main Properties:**
- `ModelPath` - model path
- `InputImage` - input image
- `ClassLabel` - class label
- `Confidence` - confidence

#### TPyObjectDetectorYolo

YOLO object detector.

**Main Properties:**
- `ModelPath` - YOLO model path
- `ConfigPath` - configuration path
- `InputImage` - input image
- `DetectedObjects` - detected objects

#### TPySegmentatorUNet

UNet segmentator.

**Main Properties:**
- `ModelPath` - UNet model path
- `InputImage` - input image
- `SegmentationMask` - segmentation mask

#### TPythonIntegration

Python integration.

**Main Methods:**
- `CallPythonFunction()` - call Python function
- `LoadPythonModule()` - load Python module

### See Also

- Source code: `Libraries/Rdk-PyMachineLearningLib/Core/`
