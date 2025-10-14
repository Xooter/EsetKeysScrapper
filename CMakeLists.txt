cmake_minimum_required(VERSION 3.16)

project(EsetKeys VERSION 1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

# ================================
# CONFIGURACIÓN WINDOWS
# ================================
if(WIN32)
    message(STATUS "Compilando para Windows...")

    set(VCPKG_TARGET_TRIPLET x64-mingw-dynamic)
    include("C:/Users/Desktop/vcpkg/scripts/buildsystems/vcpkg.cmake")

    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

    add_executable(${PROJECT_NAME}Console main.cpp ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME}Console PRIVATE
        nlohmann_json::nlohmann_json
        OpenSSL::SSL
        CURL::libcurl
    )

    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)

    add_executable(${PROJECT_NAME}Qt WIN32 main_qt.cpp ${SRC_FILES})
    target_compile_definitions(${PROJECT_NAME}Qt PRIVATE WITH_QT)
    target_link_libraries(${PROJECT_NAME}Qt PRIVATE
        Qt6::Widgets
        nlohmann_json::nlohmann_json
        OpenSSL::SSL
        CURL::libcurl
    )

# ================================
# CONFIGURACIÓN LINUX
# ================================
else()
    message(STATUS "Compilando para Linux...")

    add_executable(${PROJECT_NAME}Console main.cpp ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME}Console PRIVATE
        nlohmann_json::nlohmann_json
        OpenSSL::SSL
        CURL::libcurl
    )

    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)

    add_executable(${PROJECT_NAME}Qt main_qt.cpp ${SRC_FILES})
    target_compile_definitions(${PROJECT_NAME}Qt PRIVATE WITH_QT)
    target_link_libraries(${PROJECT_NAME}Qt PRIVATE
        Qt6::Widgets
        nlohmann_json::nlohmann_json
        OpenSSL::SSL
        CURL::libcurl
    )
endif()
