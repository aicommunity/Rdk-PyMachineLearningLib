## TPySegmentatorUNet / Protobuf — сегментаторы в Python

**Классы**: `TPySegmentatorUNet`, `TPySegmentatorProtobuf` — сегментация изображений через Python/UNet или protobuf-модель.  
**Регистрация**: `Lib.cpp` → `UploadClass("PySegmentatorUNet", ...)`, `"PySegmentatorProtobuf"`.  
**Storage-инстансы**: `ClassName` сегментатора; параметры: модель/скрипт, классы/цвета.

```mermaid
classDiagram
    TPyComponent <|-- TPySegmentatorUNet
    TPyComponent <|-- TPySegmentatorProtobuf
```

### Входы/выходы
- Вход: `UBitmap`.
- Выход: маска/карта сегментации.

```mermaid
flowchart LR
    img[UBitmap] --> seg[TPySegmentator*]
    seg --> mask[Mask]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant Cfg as Config
    participant Seg as TPySegmentatorUNet
    Cfg->>Seg: load model
    loop per frame
        Cfg-->>Seg: image
        Seg-->>Cfg: mask
    end
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## TPySegmentator* — Python segmentators

Produce segmentation masks from images using Python models (UNet/protobuf).
