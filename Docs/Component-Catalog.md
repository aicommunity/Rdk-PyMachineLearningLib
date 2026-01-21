# Rdk-PyMachineLearningLib — Component Catalog

Компоненты из `Core/Lib.cpp` (Python ML интеграция) с группами и кратким назначением.

## Base / Integration
- **TPyComponent** (упоминается в архитектуре) — базовый Python-bridge; вход: пути к скриптам/функциям; выход: результаты Python-вызовов.

## Classifiers
- **TPyUBitmapClassifier** / `PyUBitmapClassifier` — классификация bitmap через Python; вход: изображения; выход: метка/скор.
- **TPyAggregateClassifier** / `PyAggregateClassifier` — ансамбль/агрегация результатов; вход: набор результатов; выход: агрегированный класс.
- **TPyClassifierTrainer** — обучение классификаторов в Python; вход: данные/скрипт; выход: обученные веса/модель.

## Detectors
- **TPyObjectDetectorBasic** / `PyObjectDetectorBasic`, **TPyObjectDetector** / `PyObjectDetector`, **TPyObjectDetectorSqueezeDet** / `PyObjectDetectorSqueezeDet` — детекция объектов в Python; вход: изображения; выход: bounding boxes/классы.
- **TPyDetectorTrainer** — обучение детекторов; вход: датасеты/скрипты; выход: модели.
- **TPyDetPredict** — предикт детекции; вход: изображение; выход: результаты детекции.

## Segmentators
- **TPySegmentatorProtobuf**, **TPySegmentatorUNet** — сегментация (protobuf/UNet) через Python; вход: изображения; выход: маски.
- **TPySegmenterTrainer** — обучение сегментаторов; вход: датасеты/скрипты; выход: модели.

## Other
- **TPyPredictSort** — сортировка предсказаний (постобработка).

**Приоритет описания:** жизненный цикл Python-интерпретатора, подготовка данных (OpenCV/NumPy конвертеры), минимальные примеры конфигов (пути к скриптам/моделям). См. `Docs/Components/PyMLComponents.md` и детальные файлы: [`TPyComponent`](Components/TPyComponent.md), [`TPyUBitmapClassifier`](Components/TPyUBitmapClassifier.md), [`TPyObjectDetector*`](Components/TPyObjectDetector.md), [`TPySegmentator*`](Components/TPySegmentatorUNet.md), [`TPyDetectorTrainer/TPySegmenterTrainer`](Components/TPyDetectorTrainer.md).
