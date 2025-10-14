@echo off

set QT_PATH=C:\Qt\6.6.2\mingw_64
set PATH=%QT_PATH%\bin;%PATH%

if not exist build mkdir build
cd build

REM Limpiar builds previos
REM del /s /q *

cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=C:\Users\Desktop\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
cmake --build . --config Release -- -j8

cd ..
echo -------------------------------
echo Compilación finalizada
pause
