# Скрипты для сборки и управления проектом QtProjectManager

## Описание скриптов

### 1. build.sh (Linux/macOS) / build.bat (Windows)
Скрипт для сборки проекта с поддержкой различных режимов.

**Использование:**
```bash
# Linux/macOS
./build.sh                    # Сборка в режиме Release
./build.sh debug              # Сборка в режиме Debug
./build.sh release            # Сборка в режиме Release
./build.sh clean              # Очистка и сборка в режиме Release
./build.sh debug clean        # Очистка и сборка в режиме Debug

# Windows
build.bat                     # Сборка в режиме Release
build.bat debug               # Сборка в режиме Debug
build.bat release             # Сборка в режиме Release
build.bat clean               # Очистка и сборка в режиме Release
build.bat debug clean         # Очистка и сборка в режиме Debug
```

**Параметры:**
- `debug` / `Debug` — отладочная сборка
- `release` / `Release` — релизная сборка (по умолчанию)
- `clean` — очистка предыдущей сборки перед компиляцией

### 2. run.sh (Linux/macOS)
Скрипт для запуска собранного приложения.

**Использование:**
```bash
./run.sh                      # Запуск Release версии
./run.sh debug                # Запуск Debug версии
./run.sh release              # Запуск Release версии
```

Если приложение ещё не собрано, скрипт автоматически выполнит сборку.

### 3. clean.sh (Linux/macOS)
Скрипт для полной очистки проекта от временных файлов сборки.

**Использование:**
```bash
./clean.sh
```

Удаляет:
- Директорию `build/`
- Временные файлы (*.o, *.moc, *.ui_*, qrc_*.cpp)
- Файлы CMake (Makefile, CMakeCache.txt, compile_commands.json)

## Примеры использования

### Быстрая сборка и запуск:
```bash
./build.sh && ./run.sh
```

### Полная пересборка в отладочном режиме:
```bash
./build.sh debug clean
./run.sh debug
```

### Очистка и новая сборка:
```bash
./clean.sh
./build.sh release
```

## Требования

- CMake 3.16 или выше
- Qt 5 или Qt 6 с компонентами: Widgets, Xml, SerialPort
- Компилятор C++17 (GCC, Clang, MSVC)
- Для Linux/macOS: bash
- Для Windows: cmd.exe или PowerShell

## Примечания

- Все скрипты определяют директорию проекта автоматически
- Скрипты поддерживают работу из любой директории
- При сборке используется многопоточная компиляция (-j$(nproc))
- В режиме Debug включаются отладочные символы
- В режиме Release включается оптимизация
