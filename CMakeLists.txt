cmake_minimum_required(VERSION 3.16)

set(PJ_NAME EsetKeys)
project(${PJ_NAME} VERSION 1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)
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

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")

    set(CURL_ROOT_DIR "${CMAKE_SOURCE_DIR}/dependencies/windows-curl") 
    set(CURL_INCLUDE_DIR "${CURL_ROOT_DIR}/include")
    set(CURL_LIB_DIR "${CURL_ROOT_DIR}/lib")
    find_library(CURL_LIBRARY NAMES curl libcurl PATHS ${CURL_LIB_DIR} NO_DEFAULT_PATH)
    if (NOT CURL_LIBRARY)
        message(FATAL_ERROR "libcurl no encontrada en ${CURL_LIB_DIR}")
    endif()

    set(OPENSSL_ROOT_DIR "${CMAKE_SOURCE_DIR}/dependencies/windows-openssl") 
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib")
    find_library(OPENSSL_LIBRARY NAMES openssl crypto libssl ssl libcrypto PATHS ${OPENSSL_LIB_DIR} NO_DEFAULT_PATH)
    if (NOT OPENSSL_LIBRARY)
        message(FATAL_ERROR "OpenSSL no encontrada en ${OPENSSL_LIB_DIR}")
    endif()

    target_include_directories(${PJ_NAME}Console PUBLIC ${CURL_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(${PJ_NAME}Console PRIVATE
        ${CURL_LIBRARY}
        ${OPENSSL_LIBRARY}
        ws2_32
        crypt32
        nlohmann_json::nlohmann_json
    )
endif()
