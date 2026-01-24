# Rdk-PyMachineLearningLib - Документация

## RU

### Назначение

**Rdk-PyMachineLearningLib** предоставляет интеграцию с Python библиотеками машинного обучения.

### Краткий обзор

Библиотека включает компоненты для:
- Классификации изображений через Python
- Детекции объектов (YOLO, SqueezeDet)
- Сегментации изображений (UNet, Protobuf)
- Обучения моделей

### Быстрый старт

#### Детекция объектов с YOLO

```cpp
// Создание детектора YOLO
auto detector = storage->CreateComponent<TPyObjectDetectorYolo>("YOLO");
detector->ModelPath = "yolo.weights";
detector->ConfigPath = "yolo.cfg";
detector->InputImage.AttachTo(&source->OutputImage);
detector->Build();
detector->Calculate();
auto detections = detector->Detections();
```

### Связь с корневой документацией

Для обзорной информации см. корневую документацию проекта:
- `Docs/Libraries/Rdk-PyMachineLearningLib.md` - обзор библиотеки (в корневом репозитории)

### Детальная документация

- [Architecture.md](Architecture.md) - архитектура библиотеки
- [Usage-Examples.md](Usage-Examples.md) - примеры использования
- [API-Overview.md](API-Overview.md) - обзор API
- [Component-Catalog.md](Component-Catalog.md) - каталог компонентов

---

## EN

### Purpose

**Rdk-PyMachineLearningLib** provides integration with Python machine learning libraries.

### Brief Overview

The library includes components for:
- Image classification via Python
- Object detection (YOLO, SqueezeDet)
- Image segmentation (UNet, Protobuf)
- Model training

### Quick Start

#### Object Detection with YOLO

```cpp
// Create YOLO detector
auto detector = storage->CreateComponent<TPyObjectDetectorYolo>("YOLO");
detector->ModelPath = "yolo.weights";
detector->ConfigPath = "yolo.cfg";
detector->InputImage.AttachTo(&source->OutputImage);
detector->Build();
detector->Calculate();
auto detections = detector->Detections();
```

### Link to Root Documentation

For overview information see root project documentation:
- `Docs/Libraries/Rdk-PyMachineLearningLib.md` - library overview (in root repository)

### Detailed Documentation

- [Architecture.md](Architecture.md) - library architecture
- [Usage-Examples.md](Usage-Examples.md) - usage examples
- [API-Overview.md](API-Overview.md) - API overview
- [Component-Catalog.md](Component-Catalog.md) - component catalog
