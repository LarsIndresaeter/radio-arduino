
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    CONAN_BASIC_SETUP()
endif()

include_directories(../../libs/crypto/include)

add_executable(raduino-device-ws2812b main.cpp ../../libs/crypto/crypto.cpp)
target_link_libraries(raduino-device-ws2812b ${CONAN_LIBS} epollevent eventprocess monitor uart protocol pthread)

