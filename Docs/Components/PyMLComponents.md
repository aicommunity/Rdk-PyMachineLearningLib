# Py ML Components (Rdk-PyMachineLearningLib)

## Обзор

Эта страница содержит краткое описание всех компонентов библиотеки Rdk-PyMachineLearningLib с ссылками на детальную документацию.

## Базовые компоненты

- **[TPyComponent](TPyComponent.md)** — базовый компонент для всех Python-интегрированных компонентов. Предоставляет функциональность для работы с Python интерпретатором, управления GIL, загрузки Python модулей.

- **[TPyBaseTrainer](TPyBaseTrainer.md)** — базовый класс тренера через Python. Управляет жизненным циклом обучения моделей в отдельном потоке, отслеживает статус обучения, обрабатывает остановку обучения.

- **[TPythonIntegration](TPythonIntegration.md)** — вспомогательный класс интеграции с Python (пример реализации). Демонстрирует базовые принципы работы с Python через Boost.Python.

## Классификаторы

- **[TPyUBitmapClassifier](TPyUBitmapClassifier.md)** — классификатор растровых изображений через Python. Классифицирует изображения (UBitmap) используя Python модели машинного обучения.

- **[TPyAggregateClassifier](TPyAggregateClassifier.md)** — агрегатный классификатор (ансамбль). Классифицирует объекты в заданных прямоугольных областях (агрегатах) изображения.

- **[TPyClassifierTrainer](TPyClassifierTrainer.md)** — тренер классификаторов через Python. Обучение моделей классификации изображений.

## Детекторы объектов

- **[TPyObjectDetector](TPyObjectDetector.md)** — базовый детектор объектов через Python. Базовый класс для специализированных детекторов.

- **[TPyObjectDetectorYolo](TPyObjectDetectorYolo.md)** — YOLO детектор объектов. Поддерживает YOLOv2 и YOLOv3.

- **[TPyObjectDetectorYoloEx](TPyObjectDetectorYoloEx.md)** — расширенный YOLO детектор с фильтрацией и заменой классов.

- **[TPyObjectDetectorSqueezeDet](TPyObjectDetectorSqueezeDet.md)** — SqueezeDet детектор объектов.

- **[TPyDetectorTrainer](TPyDetectorTrainer.md)** — тренер детекторов через Python. Обучение моделей детекции объектов.

- **[TPyDetPredict](TPyDetPredict.md)** — предикт детекции через Python. Пакетная детекция объектов на множестве изображений в отдельном потоке.

## Сегментаторы

- **[TPySegmentator](TPySegmentator.md)** — базовый сегментатор через Python. Базовый класс для специализированных сегментаторов.

- **[TPySegmentatorUNet](TPySegmentatorUNet.md)** — U-Net сегментатор через Python. Семантическая сегментация изображений используя U-Net архитектуру.

- **[TPySegmentatorProtobuf](TPySegmentatorProtobuf.md)** — сегментатор с использованием Protobuf модели.

- **[TPySegmenterTrainer](TPySegmenterTrainer.md)** — тренер сегментаторов через Python. Обучение моделей сегментации изображений.

## Вспомогательные компоненты

- **[TPyPredictSort](TPyPredictSort.md)** — сортировка предсказаний через Python. Предсказание и сортировка изображений по классам в отдельные папки.

## См. также

- [README.md](README.md) — обзор документации компонентов
- [Component-Catalog.md](../Component-Catalog.md) — каталог компонентов
- [Architecture.md](../Architecture.md) — архитектура библиотеки
- [API-Overview.md](../API-Overview.md) — обзор API
