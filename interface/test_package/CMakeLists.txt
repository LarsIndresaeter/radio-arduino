cmake_minimum_required(VERSION 3.5)
project(unittest)

set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
endif()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(
    unittest unittest.cpp
    fakeCryptoHandler.cpp
    fakeBufferProtocol.cpp
    fakeStreamProtocol.cpp
    bothProtocols.cpp
    protocolBaseTest.cpp
    arduinoProtocolTest.cpp
    linuxProtocolTest.cpp
    commandTest.cpp
)

target_include_directories(unittest PRIVATE ${CMAKE_BINARY_DIR}/libs/protocol/include)
target_include_directories(unittest PRIVATE ${CMAKE_BINARY_DIR}/libs/commands/include)
target_link_libraries(unittest ${GTEST_LIBRARIES} pthread)

enable_testing()

add_test(
    NAME static_test
    COMMAND $<TARGET_FILE:unittest>
    )
