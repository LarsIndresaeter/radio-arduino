
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    CONAN_BASIC_SETUP()
endif()

include_directories(../../libs/crypto/include)

find_package(PahoMqttCpp REQUIRED)
find_package(nlohmann_json REQUIRED)
add_definitions(-DUSE_FEATURE_MQTT=1)

add_executable(raduino-power-monitor-mqtt currentMonitor.cpp mqtt-common.cpp main.cpp ../../libs/crypto/crypto.cpp)
target_link_libraries(raduino-power-monitor-mqtt ${CONAN_LIBS} epollevent eventprocess monitor uart protocol radiosession pthread)

