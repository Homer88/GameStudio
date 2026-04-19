#!/bin/bash
# Скрипт для очистки проекта QtProjectManager
# Удаляет директорию сборки и временные файлы

set -e

# Цвета для вывода
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Определение директорий
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo -e "${YELLOW}=== Очистка проекта QtProjectManager ===${NC}"
echo "Директория сборки: ${BUILD_DIR}"

# Удаление директории сборки
if [ -d "${BUILD_DIR}" ]; then
    echo -e "${YELLOW}Удаление директории build...${NC}"
    rm -rf "${BUILD_DIR}"
    echo -e "${GREEN}Директория build удалена.${NC}"
else
    echo "Директория build не найдена."
fi

# Удаление других временных файлов (если есть)
TEMP_FILES=(
    "*.o"
    "*.moc"
    "*.ui_*"
    "qrc_*.cpp"
    "Makefile"
    "CMakeCache.txt"
    "compile_commands.json"
)

echo -e "${YELLOW}Поиск временных файлов...${NC}"
for pattern in "${TEMP_FILES[@]}"; do
    find "${SCRIPT_DIR}" -name "$pattern" -type f -delete 2>/dev/null || true
done

echo -e "${GREEN}=== Очистка завершена! ===${NC}"
