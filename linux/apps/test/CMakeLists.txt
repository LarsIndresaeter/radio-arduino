
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    CONAN_BASIC_SETUP()
endif()

include_directories(../../libs/crypto/include)

if(mqtt)
    find_package(PahoMqttCpp REQUIRED)
    add_definitions(-DUSE_FEATURE_MQTT=1)
endif()

add_executable(test-tool main.cpp ../../libs/crypto/crypto.cpp)
target_link_libraries(test-tool ${CONAN_LIBS} epollevent eventprocess monitor uart protocol pthread)

