## TPyUBitmapClassifier — Python классификатор изображений

**Класс**: `TPyUBitmapClassifier` (`PyUBitmapClassifier`) — вызывает Python-код для классификации bitmap.  
**Регистрация**: `Lib.cpp` → `UploadClass("PyUBitmapClassifier", ...)`.  
**Storage-инстансы**: `ClassName = "PyUBitmapClassifier"`; параметры: путь к скрипту/модели, имя функции.

```mermaid
classDiagram
    TPyComponent <|-- TPyUBitmapClassifier
    class TPyUBitmapClassifier {
        +scriptPath : string
        +function : string
    }
```

### Входы/выходы
- Вход: `UBitmap` изображение.
- Выход: класс/скор.

```mermaid
flowchart LR
    img[UBitmap] --> cls[TPyUBitmapClassifier]
    cls --> label[Class/score]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant Cfg as Config
    participant Cls as TPyUBitmapClassifier
    Cfg->>Cls: script + model path
    loop per image
        Cfg-->>Cls: UBitmap
        Cls-->>Cfg: class/score
    end
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## TPyUBitmapClassifier — Python image classifier

Runs Python inference on bitmaps; outputs class/score from provided script/model.
