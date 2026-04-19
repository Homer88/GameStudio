#!/bin/bash
# Скрипт для запуска проекта QtProjectManager
# Использование: ./run.sh [debug|release]

set -e

# Цвета для вывода
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Параметры по умолчанию
BUILD_TYPE="Release"

# Обработка аргументов
if [ "$1" == "debug" ] || [ "$1" == "Debug" ]; then
    BUILD_TYPE="Debug"
elif [ "$1" == "release" ] || [ "$1" == "Release" ]; then
    BUILD_TYPE="Release"
fi

# Определение директорий
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
EXECUTABLE="${BUILD_DIR}/QtProjectManager"

echo -e "${GREEN}=== Запуск QtProjectManager ===${NC}"
echo "Тип сборки: ${BUILD_TYPE}"

# Проверка существования исполняемого файла
if [ ! -f "${EXECUTABLE}" ]; then
    echo -e "${YELLOW}Исполняемый файл не найден. Выполняется сборка...${NC}"
    "${SCRIPT_DIR}/build.sh" ${BUILD_TYPE,,}
fi

# Запуск приложения
echo -e "${YELLOW}Запуск приложения...${NC}"
cd "${BUILD_DIR}"
./QtProjectManager "$@"
