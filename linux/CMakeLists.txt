cmake_minimum_required(VERSION 2.8)
project(epoll_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    CONAN_BASIC_SETUP()
    if(NOT "${CONAN_RES_DIRS_RADUINO-AVR}" STREQUAL "")
        configure_file("${CONAN_RES_DIRS_RADUINO-AVR}/arduino.hex" "${CMAKE_BINARY_DIR}/bin/arduino.hex")
    endif()
endif()

add_subdirectory(libs)
add_subdirectory(apps)

