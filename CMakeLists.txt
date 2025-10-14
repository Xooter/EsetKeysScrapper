# cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=C:\Users\Desktop\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
# cmake --build build --config Release

cmake_minimum_required(VERSION 3.16)

set(PJ_NAME EsetKeys)
project(${PJ_NAME} VERSION 1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

set(VCPKG_TARGET_TRIPLET x64-mingw-dynamic)
set(MINGW TRUE)
include("C:/Users/Desktop/vcpkg/scripts/buildsystems/vcpkg.cmake")

file(GLOB_RECURSE SRC_FILES src/*.cpp)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

add_executable(${PJ_NAME}Console main.cpp ${SRC_FILES})
target_link_libraries(${PJ_NAME}Console PRIVATE
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    CURL::libcurl
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PJ_NAME}Qt main_qt.cpp ${SRC_FILES})
target_compile_definitions(${PJ_NAME}Qt PRIVATE WITH_QT)
target_link_libraries(${PJ_NAME}Qt PRIVATE
    Qt6::Widgets
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    CURL::libcurl
)