
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    CONAN_BASIC_SETUP()
endif()

include_directories(../../libs/crypto/include)

find_package(PahoMqttCpp REQUIRED)
find_package(nlohmann_json REQUIRED)
add_definitions(-DUSE_FEATURE_MQTT=1)

include_directories(raduino-mqtt-proxy include)
add_executable(raduino-mqtt-proxy main.cpp commandCallback.cpp)
target_link_libraries(raduino-mqtt-proxy ${CONAN_LIBS} pthread)

