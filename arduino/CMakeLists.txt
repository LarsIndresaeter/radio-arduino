cmake_minimum_required(VERSION 3.5)
project(avr-uart)

set(PRODUCT_NAME arduino)

set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_SOURCE_DIR}/cmake/avr-vars.cmake)

# Conanify this CMake file
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
endif()

add_subdirectory(include)
add_subdirectory(third_party_libs)
add_subdirectory(libs)
add_subdirectory(src)
