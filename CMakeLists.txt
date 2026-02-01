cmake_minimum_required(VERSION 3.16)

project(EsetKeys VERSION 1.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opción para controlar si se compila la versión Qt
option(BUILD_QT_VERSION "Build Qt GUI version" ON)

# Recopilar archivos fuente
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Buscar dependencias base (siempre necesarias)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

# ================================
# CONFIGURACIÓN WINDOWS
# ================================
if(WIN32)
    message(STATUS "Compilando para Windows con MinGW...")
    
    # Flags para linkeo estático (incluir todas las librerías)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    
    # Flags adicionales para Windows
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
    
    # ================================
    # VERSIÓN CONSOLA WINDOWS
    # ================================
    add_executable(${PROJECT_NAME}Console main.cpp ${SRC_FILES})
    
    target_link_libraries(${PROJECT_NAME}Console PRIVATE
        nlohmann_json::nlohmann_json
        OpenSSL::SSL
        OpenSSL::Crypto
        CURL::libcurl
        ws2_32
        crypt32
        bcrypt
    )
    
    # Configuración específica para consola Windows
    set_target_properties(${PROJECT_NAME}Console PROPERTIES
        OUTPUT_NAME "EsetKeysConsole"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    
    # ================================
    # VERSIÓN QT WINDOWS
    # ================================
    if(BUILD_QT_VERSION)
        find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)
        
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTORCC ON)
        
        add_executable(${PROJECT_NAME}Qt WIN32 main_qt.cpp ${SRC_FILES})
        
        target_compile_definitions(${PROJECT_NAME}Qt PRIVATE WITH_QT)
        
        target_link_libraries(${PROJECT_NAME}Qt PRIVATE
            Qt6::Widgets
            Qt6::Core
            Qt6::Gui
            nlohmann_json::nlohmann_json
            OpenSSL::SSL
            OpenSSL::Crypto
            CURL::libcurl
            ws2_32
            crypt32
            bcrypt
        )
        
        set_target_properties(${PROJECT_NAME}Qt PROPERTIES
            OUTPUT_NAME "EsetKeysQt"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
    endif()

# ================================
# CONFIGURACIÓN LINUX
# ================================
else()
    message(STATUS "Compilando para Linux...")
    
    # ================================
    # VERSIÓN CONSOLA LINUX
    # ================================
    add_executable(${PROJECT_NAME}Console main.cpp ${SRC_FILES})
    
    target_link_libraries(${PROJECT_NAME}Console PRIVATE
        nlohmann_json::nlohmann_json
        OpenSSL::SSL
        OpenSSL::Crypto
        CURL::libcurl
        pthread
    )
    
    set_target_properties(${PROJECT_NAME}Console PROPERTIES
        OUTPUT_NAME "EsetKeysConsole"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    
    # ================================
    # VERSIÓN QT LINUX
    # ================================
    if(BUILD_QT_VERSION)
        find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)
        
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTORCC ON)
        
        add_executable(${PROJECT_NAME}Qt main_qt.cpp ${SRC_FILES})
        
        target_compile_definitions(${PROJECT_NAME}Qt PRIVATE WITH_QT)
        
        target_link_libraries(${PROJECT_NAME}Qt PRIVATE
            Qt6::Widgets
            Qt6::Core
            Qt6::Gui
            nlohmann_json::nlohmann_json
            OpenSSL::SSL
            OpenSSL::Crypto
            CURL::libcurl
            pthread
        )
        
        set_target_properties(${PROJECT_NAME}Qt PROPERTIES
            OUTPUT_NAME "EsetKeysQt"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
    endif()
endif()

# ================================
# MENSAJES INFORMATIVOS
# ================================
if(WIN32)
    message(STATUS "====================================")
    message(STATUS "Plataforma: Windows (MinGW)")
    message(STATUS "Linkeo: Estático (libs incluidas)")
else()
    message(STATUS "====================================")
    message(STATUS "Plataforma: Linux")
endif()

if(BUILD_QT_VERSION)
    message(STATUS "Versiones: Console + Qt GUI")
else()
    message(STATUS "Versiones: Solo Console (sin Qt)")
endif()
message(STATUS "====================================")
