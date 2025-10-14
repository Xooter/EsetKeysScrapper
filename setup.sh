#!/bin/bash
set -e

PROJECT_NAME=EsetKeys
BUILD_DIR=build

echo "==============================="
echo " Compilando proyecto $PROJECT_NAME para Linux"
echo "==============================="

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -- -j"$(nproc)"

cd ..
echo "-------------------------------"
echo "✅ Compilación finalizada correctamente"
echo "📁 Binarios generados en: $BUILD_DIR"
echo "-------------------------------"
