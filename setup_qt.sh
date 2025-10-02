#!/bin/bash
set -e

PROJECT_NAME=EsetKeys
BUILD_DIR=build_qt

echo "=== Build & Run Qt GUI version ==="

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -- -j$(nproc)

export QT_QPA_PLATFORM=xcb

echo "Running Qt GUI executable..."
./EsetKeysQt
