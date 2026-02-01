# ================================
# Toolchain para Cross-Compilación a Windows
# ================================
# Uso: cmake -DCMAKE_TOOLCHAIN_FILE=windows-toolchain.cmake ..

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Compiladores MinGW
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Flags para linkeo estático
set(CMAKE_EXE_LINKER_FLAGS_INIT "-static -static-libgcc -static-libstdc++")

# Configuración de búsqueda de bibliotecas
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Extensiones de archivos para Windows
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")

# Optimizaciones adicionales
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -s")
