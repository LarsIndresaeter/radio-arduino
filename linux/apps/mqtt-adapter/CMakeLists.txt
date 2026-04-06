
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    CONAN_BASIC_SETUP()
endif()

include_directories(../../libs/crypto/include)

find_package(PahoMqttCpp REQUIRED)
find_package(nlohmann_json REQUIRED)
add_definitions(-DUSE_FEATURE_MQTT=1)

include_directories(raduino-mqtt-adapter include)
add_executable(raduino-mqtt-adapter main.cpp commandCallback.cpp deviceController.cpp ../../libs/crypto/crypto.cpp)
target_link_libraries(raduino-mqtt-adapter ${CONAN_LIBS} epollevent eventprocess monitor uart protocol radiosession version pthread)

