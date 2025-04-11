
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    CONAN_BASIC_SETUP()
endif()

include_directories(../../libs/crypto/include)

find_package(PahoMqttCpp REQUIRED)
find_package(nlohmann_json REQUIRED)
add_definitions(-DUSE_FEATURE_MQTT=1)

include_directories(mqtt-client include)
add_executable(mqtt-client main.cpp desiredStateCallback.cpp desiredStateConfiguration.cpp actualState.cpp mqtt_common.cpp radioSession.cpp digitalTwin.cpp ../../libs/crypto/crypto.cpp)
target_link_libraries(mqtt-client ${CONAN_LIBS} epollevent eventprocess monitor uart protocol pthread)

