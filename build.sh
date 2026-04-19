#!/bin/bash
# Скрипт для сборки проекта QtProjectManager
# Использование: ./build.sh [debug|release] [clean]

set -e

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Параметры по умолчанию
BUILD_TYPE="Release"
CLEAN_BUILD=false

# Обработка аргументов
while [[ $# -gt 0 ]]; do
    case $1 in
        debug|Debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        release|Release)
            BUILD_TYPE="Release"
            shift
            ;;
        clean)
            CLEAN_BUILD=true
            shift
            ;;
        *)
            echo -e "${RED}Неизвестный параметр: $1${NC}"
            echo "Использование: $0 [debug|release] [clean]"
            exit 1
            ;;
    esac
done

# Определение директорий
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
SOURCE_DIR="${SCRIPT_DIR}"

echo -e "${GREEN}=== Сборка QtProjectManager ===${NC}"
echo "Тип сборки: ${BUILD_TYPE}"
echo "Директория проекта: ${SOURCE_DIR}"
echo "Директория сборки: ${BUILD_DIR}"

# Очистка при необходимости
if [ "$CLEAN_BUILD" = true ]; then
    echo -e "${YELLOW}Очистка предыдущей сборки...${NC}"
    rm -rf "${BUILD_DIR}"
fi

# Создание директории сборки
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# Конфигурация CMake
echo -e "${YELLOW}Конфигурация CMake...${NC}"
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      "${SOURCE_DIR}"

# Сборка проекта
echo -e "${YELLOW}Компиляция проекта...${NC}"
cmake --build . --config ${BUILD_TYPE} -j$(nproc)

# Проверка результата
if [ $? -eq 0 ]; then
    echo -e "${GREEN}=== Сборка успешно завершена! ===${NC}"
    echo "Исполняемый файл: ${BUILD_DIR}/QtProjectManager"
    
    # Показать информацию о файле
    if [ -f "${BUILD_DIR}/QtProjectManager" ]; then
        ls -lh "${BUILD_DIR}/QtProjectManager"
    fi
else
    echo -e "${RED}=== Ошибка сборки! ===${NC}"
    exit 1
fi
