@echo off
REM Скрипт для сборки проекта QtProjectManager (Windows)
REM Использование: build.bat [debug|release] [clean]

setlocal enabledelayedexpansion

REM Параметры по умолчанию
set BUILD_TYPE=Release
set CLEAN_BUILD=false

REM Обработка аргументов
:parse_args
if "%~1"=="" goto :end_parse_args
if /i "%~1"=="debug" (
    set BUILD_TYPE=Debug
    shift
    goto :parse_args
)
if /i "%~1"=="release" (
    set BUILD_TYPE=Release
    shift
    goto :parse_args
)
if /i "%~1"=="clean" (
    set CLEAN_BUILD=true
    shift
    goto :parse_args
)
echo Неизвестный параметр: %~1
echo Использование: %~nx0 [debug^|release] [clean]
exit /b 1

:end_parse_args

REM Определение директорий
set SCRIPT_DIR=%~dp0
set BUILD_DIR=%SCRIPT_DIR%build
set SOURCE_DIR=%SCRIPT_DIR%

echo.
echo === Сборка QtProjectManager ===
echo Тип сборки: %BUILD_TYPE%
echo Директория проекта: %SOURCE_DIR%
echo Директория сборки: %BUILD_DIR%
echo.

REM Очистка при необходимости
if "%CLEAN_BUILD%"=="true" (
    echo Очистка предыдущей сборки...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
)

REM Создание директории сборки
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%"

REM Конфигурация CMake
echo Конфигурация CMake...
cmake -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ^
      "%SOURCE_DIR%"
if errorlevel 1 (
    echo === Ошибка конфигурации CMake! ===
    exit /b 1
)

REM Сборка проекта
echo.
echo Компиляция проекта...
cmake --build . --config %BUILD_TYPE%
if errorlevel 1 (
    echo === Ошибка сборки! ===
    exit /b 1
)

REM Проверка результата
echo.
echo === Сборка успешно завершена! ===
if exist "%BUILD_DIR%\QtProjectManager.exe" (
    echo Исполняемый файл: %BUILD_DIR%\QtProjectManager.exe
    dir "%BUILD_DIR%\QtProjectManager.exe"
) else if exist "%BUILD_DIR%\QtProjectManager" (
    echo Исполняемый файл: %BUILD_DIR%\QtProjectManager
    dir "%BUILD_DIR%\QtProjectManager"
)

endlocal
