@echo off

set QT_PATH=C:\Qt\6.6.2\mingw_64
set PATH=%QT_PATH%\bin;%PATH%

if not exist build mkdir build
cd build

REM Limpiar builds previos
REM del /s /q *

cmake -G "MinGW Makefiles" ..

cmake --build . --config Release

cd ..
echo -------------------------------
echo Compilación finalizada
pause
