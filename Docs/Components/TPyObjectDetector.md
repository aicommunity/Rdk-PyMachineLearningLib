## TPyObjectDetector / Basic / SqueezeDet — Python детекторы

**Классы**: `TPyObjectDetector`, `TPyObjectDetectorBasic`, `TPyObjectDetectorSqueezeDet` — детекция объектов через Python-модели.  
**Регистрация**: `Lib.cpp` → `UploadClass("PyObjectDetector", ...)`, `"PyObjectDetectorBasic"`, `"PyObjectDetectorSqueezeDet"`.  
**Storage-инстансы**: `ClassName` соответствующего детектора; параметры: путь к модели/скрипту, имена функций препро/постпроцессинга.

```mermaid
classDiagram
    TPyComponent <|-- TPyObjectDetector
    TPyObjectDetector <|-- TPyObjectDetectorBasic
    TPyObjectDetector <|-- TPyObjectDetectorSqueezeDet
```

### Входы/выходы
- Вход: `UBitmap` изображение.
- Выход: bounding boxes + классы + scores.

```mermaid
flowchart LR
    img[UBitmap] --> det[TPyObjectDetector*]
    det --> boxes[Boxes/classes]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant Cfg as Config
    participant Det as TPyObjectDetector
    Cfg->>Det: load model/script
    loop per frame
        Cfg-->>Det: image
        Det-->>Cfg: detections
    end
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## TPyObjectDetector* — Python object detectors

Run Python detection models (generic/basic/SqueezeDet) on images, returning boxes and classes.
