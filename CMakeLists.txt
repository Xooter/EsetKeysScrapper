cmake_minimum_required(VERSION 3.16)

project(EsetKeys VERSION 1.3 LANGUAGES CXX)

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

    set(VCPKG_TARGET_TRIPLET x64-mingw-static)
    include("C:/Users/Desktop/vcpkg/scripts/buildsystems/vcpkg.cmake")

    # VCPKGS
    # curl:x64-mingw-dynamic
    # curl[ssl]:x64-mingw-dynamic
    # curl[sspi]:x64-mingw-dynamic
    # curl[tool]:x64-mingw-dynamic
    # nlohmann-json:x64-mingw-dynamic
    # qtbase:x64-mingw-dynamic
    # qttools:x64-mingw-dynamic
		#
		# vcpkg install curl:x64-mingw-static
	  # vcpkg install nlohmann-json:x64-mingw-static
	  # vcpkg install qtbase:x64-mingw-static
	  # vcpkg install qttools:x64-mingw-static
	  # vcpkg install openssl:x64-mingw-static

		set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
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
				Qt6::Core
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
				Qt6::Core
        nlohmann_json::nlohmann_json
        OpenSSL::SSL
        CURL::libcurl
    )
endif()
