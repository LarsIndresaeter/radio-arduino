include_directories(include)

add_library(ina219
    ina219.cpp
    )

target_link_libraries(ina219 i2c)

target_include_directories(ina219 PUBLIC ${CONAN_INCLUDE_DIRS_RADUINO-API})
target_include_directories(ina219 INTERFACE include)
