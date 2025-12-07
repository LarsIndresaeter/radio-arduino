include_directories(include)

add_library(lsm303d
    lsm303d.cpp
    command-handlers-lsm303d.cpp
    )

target_link_libraries(lsm303d i2c protocol)

target_include_directories(lsm303d PUBLIC ${CONAN_INCLUDE_DIRS_RADUINO-API})
target_include_directories(lsm303d INTERFACE include)
