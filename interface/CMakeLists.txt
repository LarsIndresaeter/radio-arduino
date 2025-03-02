cmake_minimum_required(VERSION 3.5)
project(uart-api)

set(PRODUCT_NAME api)

set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs)
