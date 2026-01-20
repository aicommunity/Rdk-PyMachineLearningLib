# ML-пайплайн интеграции Python

## RU

### Типичный ML-пайплайн

```mermaid
flowchart TB
    Input[Входное изображение]
    Preprocess[Предобработка<br/>UBAResize]
    PythonBridge[Python Bridge<br/>TPyComponent]
    PythonML[Python ML Library<br/>TensorFlow/PyTorch]
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

## EN

### Typical ML Pipeline

### ML Pipeline Execution Sequence

### YOLO Detector Integration
