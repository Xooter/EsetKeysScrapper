cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(EsetKeys VERSION 1.0)
find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)

# Establecer las rutas de inclusión para los archivos de origen y los archivos de la biblioteca json
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${nlohmann_json_INCLUDE_DIRS})

# Recopilar los archivos fuente
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Crear el ejecutable
add_executable(EsetKeys main.cpp ${SRC_FILES})

# Enlazar las bibliotecas necesarias
target_link_libraries(EsetKeys PRIVATE curl nlohmann_json::nlohmann_json OpenSSL::SSL)
