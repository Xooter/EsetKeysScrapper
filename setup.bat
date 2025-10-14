@echo off
setlocal enabledelayedexpansion

set PROJECT_NAME=EsetKeys
set QT_PATH=C:\Qt\6.6.2\mingw_64
set VCPKG_PATH=C:\Users\Desktop\vcpkg
set BUILD_DIR=build

set PATH=%QT_PATH%\bin;%PATH%

echo ================================
echo Compilando proyecto %PROJECT_NAME% para Windows
echo ================================

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

echo -- Limpiando build anterior --
del /s /q * >nul 2>&1

cmake -S .. -B . ^
  -G "MinGW Makefiles" ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_TOOLCHAIN_FILE=%VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic

if %errorlevel% neq 0 (
  echo ❌ Error en la configuración de CMake
  pause
  exit /b %errorlevel%
)

echo -- Compilando ejecutables --
cmake --build . --config Release -- -j8

if %errorlevel% neq 0 (
  echo ❌ Error al compilar
  pause
  exit /b %errorlevel%
)

cd ..
echo -------------------------------
echo ✅ Compilación finalizada correctamente
echo 📁 Binarios generados en: %BUILD_DIR%
echo -------------------------------
pause
