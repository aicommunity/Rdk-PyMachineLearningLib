# ML-пайплайн интеграции Python

## RU

### Типичный ML-пайплайн

```mermaid
flowchart TB
    Input[Входное_изображение]
    Preprocess["Предобработка (UBAResize)"]
    PythonBridge["Python_Bridge (TPyComponent)"]
    PythonML["Python_ML_Library (TensorFlow/PyTorch)"]
    Postprocess[Постобработка]
    Output[Результат]
    
    Input --> Preprocess
    Preprocess --> PythonBridge
    PythonBridge --> PythonML
    PythonML --> PythonBridge
    PythonBridge --> Postprocess
    Postprocess --> Output
```

### Последовательность выполнения ML-пайплайна

```mermaid
sequenceDiagram
    participant Component as TPyComponent
    participant Bridge as Python Bridge
    participant Python as Python ML
    participant Engine as Rdk Engine
    
    Component->>Component: PrepareInput()
    Component->>Bridge: CallPythonFunction()
    Bridge->>Python: LoadModel()
    Bridge->>Python: Predict(input)
    Python->>Python: ML Inference
    Python-->>Bridge: Result
    Bridge-->>Component: ReturnResult()
    Component->>Engine: UpdateOutput()
```

### Интеграция YOLO детектора

```mermaid
sequenceDiagram
    participant Image as Изображение
    participant YOLO as TPyObjectDetectorYolo
    participant Python as Python YOLO
    participant Engine as Rdk Engine
    
    Image->>YOLO: InputImage
    YOLO->>Python: LoadYOLOModel()
    YOLO->>Python: Detect(image)
    Python->>Python: YOLO Inference
    Python-->>YOLO: DetectedObjects
    YOLO->>Engine: UpdateProperties()
    Engine->>Engine: NotifyComponents()
```

---

Диаграммы показывают типичный ML‑пайплайн с использованием Python‑моста: входное изображение проходит предобработку (например, изменение размера через `UBAResize`), затем передаётся в Python‑компонент (`TPyComponent`), который вызывает Python ML библиотеку (TensorFlow/PyTorch), выполняет инференс и возвращает результат обратно в C++ код для постобработки и вывода.

Специальная диаграмма для YOLO детектора детализирует процесс: компонент `TPyObjectDetectorYolo` загружает YOLO модель в Python, выполняет детекцию на изображении, получает список обнаруженных объектов и обновляет свойства компонента, что уведомляет движок о новых данных.

## EN

### Typical ML Pipeline

The diagrams show a typical ML pipeline using a Python bridge: input image undergoes preprocessing (e.g., resizing via `UBAResize`), then is passed to a Python component (`TPyComponent`), which calls a Python ML library (TensorFlow/PyTorch), performs inference, and returns the result back to C++ code for postprocessing and output.

### ML Pipeline Execution Sequence

The sequence diagram details the execution flow: component prepares input, calls Python bridge, which loads the model and performs prediction, then returns results to the component for output property updates.

### YOLO Detector Integration

The special diagram for YOLO detector details the process: `TPyObjectDetectorYolo` component loads YOLO model in Python, performs detection on image, receives a list of detected objects, and updates component properties, which notifies the engine about new data.
