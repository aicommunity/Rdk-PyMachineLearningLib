# Примеры использования Rdk-PyMachineLearningLib

## RU

### Пример 1: Классификация изображений

```cpp
// Создание классификатора через Python
auto classifier = storage->CreateComponent<TPyUBitmapClassifier>();
classifier->PythonScript = "classify.py";
classifier->ModelPath = "model.h5";
classifier->Build();

// Классификация
classifier->InputImage = inputImage;
classifier->Calculate();
auto classLabel = classifier->ClassLabel;
```

### Пример 2: Детекция объектов YOLO

```cpp
// Создание YOLO детектора
auto detector = storage->CreateComponent<TPyObjectDetectorYolo>();
detector->ModelPath = "yolo.h5";
detector->ConfigPath = "yolo_config.json";
detector->Build();

// Детекция
detector->InputImage = inputImage;
detector->Calculate();
auto objects = detector->DetectedObjects;
```

### Пример 3: Сегментация UNet

```cpp
// Создание сегментатора UNet
auto segmentator = storage->CreateComponent<TPySegmentatorUNet>();
segmentator->ModelPath = "unet.h5";
segmentator->Build();

// Сегментация
segmentator->InputImage = inputImage;
segmentator->Calculate();
auto segmentation = segmentator->SegmentationMask;
```

---

## EN

### Example 1: Image Classification

```cpp
// Creating classifier via Python
auto classifier = storage->CreateComponent<TPyUBitmapClassifier>();
classifier->PythonScript = "classify.py";
classifier->ModelPath = "model.h5";
classifier->Build();

// Classification
classifier->InputImage = inputImage;
classifier->Calculate();
auto classLabel = classifier->ClassLabel;
```

### Example 2: YOLO Object Detection

```cpp
// Creating YOLO detector
auto detector = storage->CreateComponent<TPyObjectDetectorYolo>();
detector->ModelPath = "yolo.h5";
detector->ConfigPath = "yolo_config.json";
detector->Build();

// Detection
detector->InputImage = inputImage;
detector->Calculate();
auto objects = detector->DetectedObjects;
```

### Example 3: UNet Segmentation

```cpp
// Creating UNet segmentator
auto segmentator = storage->CreateComponent<TPySegmentatorUNet>();
segmentator->ModelPath = "unet.h5";
segmentator->Build();

// Segmentation
segmentator->InputImage = inputImage;
segmentator->Calculate();
auto segmentation = segmentator->SegmentationMask;
```
