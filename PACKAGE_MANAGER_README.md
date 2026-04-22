# Менеджер пакетов для редактора кода

## Обзор

Реализована система управления пакетами для кроссплатформенного редактора кода на C++/Qt.

## Структура пакета

Пакет представляет собой ZIP-архив со следующей структурой:

```
package-name.zip
├── config.xml          # Файл конфигурации (обязательно)
├── keywords.txt        # Файл кодовых слов (обязательно)
├── compiler/           # Папка с компилятором (опционально)
├── lib/                # Папка с библиотеками (*.lib, *.a)
├── include/            # Папка с заголовками (*.h, *.inc)
└── help/               # Папка с документацией (опционально)
    ├── manual.html
    ├── reference.chm
    ├── readme.md
    └── guide.pdf
```

## Обязательные файлы

1. **config.xml** - файл конфигурации пакета
   ```xml
   <?xml version="1.0"?>
   <package>
       <name>PackageName</name>
       <version>1.0.0</version>
       <description>Description of the package</description>
       <author>Author Name</author>
   </package>
   ```

2. **keywords.txt** - файл кодовых слов для подсветки синтаксиса
   - Формат: одно ключевое слово на строку
   - Или XML формат с категориями

3. **include/** - папка с заголовочными файлами (.h, .inc)

## Опциональные компоненты

- **compiler/** - компилятор и связанные инструменты
- **lib/** - библиотеки (.lib, .a, .so, .dll)
- **help/** - документация в форматах: HTML, CHM, Markdown, PDF

## Компоненты системы

### 1. PackageParser (`src/core/packageparser.h/cpp`)

Класс для анализа и распаковки пакетов:

```cpp
PackageParser parser;
PackageContent content;

// Анализ архива без распаковки
if (parser.parseArchive("package.zip", content)) {
    // Пакет валиден
}

// Распаковка в указанную директорию
if (parser.extractTo("package.zip", "/target/dir", content)) {
    // Успешно распаковано
}
```

### 2. PackageManager (`src/core/packagemanager.h/cpp`)

Управление установленными пакетами:

```cpp
PackageManager manager;
manager.initialize("~/.codeeditor/packages");

// Установка пакета
QString error;
if (manager.installPackage("package.zip", error)) {
    // Успешно установлено
}

// Удаление пакета
manager.uninstallPackage("package_id", error);

// Получение списка пакетов
QList<InstalledPackage> packages = manager.getInstalledPackages();

// Пути для компиляции
QStringList includePaths = manager.getActiveIncludePaths();
QStringList libPaths = manager.getActiveLibPaths();
```

### 3. PackageDialog (`src/core/packagedialog.h/cpp`)

Диалоговое окно менеджера пакетов:

- Просмотр установленных пакетов
- Установка новых пакетов из ZIP-архивов
- Удаление установленных пакетов
- Отображение информации о пакете
- Индикатор прогресса установки

## Использование в редакторе

### Меню "Settings"

Добавлены два пункта меню:

1. **Install Package...** - открытие диалога установки пакета
2. **Uninstall Package...** - открытие диалога удаления пакета

Оба пункта открывают единое окно менеджера пакетов.

### Хранение пакетов

Пакеты устанавливаются в директорию:
- Linux/macOS: `~/.codeeditor/packages/`
- Windows: `C:\Users\<User>\.codeeditor\packages\`

Каждый пакет устанавливается в подпапку с уникальным ID:
```
packages/
├── gcc_arm_1_0_0/
├── qt_widgets_5_15_0/
└── packages.xml  # индекс установленных пакетов
```

## Формат индекса пакетов (packages.xml)

```xml
<?xml version="1.0"?>
<packages>
    <package id="gcc_arm_1_0_0">
        <name>GCC ARM</name>
        <version>1.0.0</version>
        <installPath>/home/user/.codeeditor/packages/gcc_arm_1_0_0</installPath>
        <configPath>config.xml</configPath>
        <keywordsPath>keywords.txt</keywordsPath>
        <compilerPath>compiler/bin</compilerPath>
        <libPath>lib</libPath>
        <includePath>include</includePath>
        <helpPath>help</helpPath>
        <isActive>true</isActive>
        <installDate>2024-01-15T10:30:00</installDate>
    </package>
</packages>
```

## Расширение функциональности

### Интеграция с системой сборки

Для использования путей из пакетов в системе сборки:

```cpp
// В mainwindow.cpp или сборщике проектов
QStringList includePaths = m_packageManager->getActiveIncludePaths();
QStringList libPaths = m_packageManager->getActiveLibPaths();

// Добавить в настройки компилятора проекта
for (const QString &path : includePaths) {
    compilerArgs << "-I" << path;
}
for (const QString &path : libPaths) {
    linkerArgs << "-L" << path;
}
```

### Поддержка новых форматов помощи

В `PackageParser::detectHelpFormat()` можно добавить новые форматы:

```cpp
else if (ext == "epub")
    return PackageContent::HelpFile::EPUB;
```

## Создание тестового пакета

```bash
# Создаем структуру пакета
mkdir -p mypackage/{compiler/bin,lib,include,help}

# Создаем обязательные файлы
cat > mypackage/config.xml << EOF
<?xml version="1.0"?>
<package>
    <name>MyPackage</name>
    <version>1.0.0</version>
    <description>Test package</description>
</package>
EOF

echo "keyword1\nkeyword2\nkeyword3" > mypackage/keywords.txt
echo "// Header file" > mypackage/include/mylib.h

# Упаковываем в ZIP
cd mypackage && zip -r ../mypackage.zip .
```

## Зависимости Qt

- Qt Widgets
- Qt Xml (для чтения конфигов)
- Qt Core (QZipReader/QZipWriter доступны в Qt5+)

## Примечания

- Для работы QZipReader/QZipWriter требуется Qt5 или выше
- На некоторых платформах может потребоваться отдельная библиотека zlib
- Права доступа к файлам сохраняются при распаковке
