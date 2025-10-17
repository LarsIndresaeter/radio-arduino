include_directories(include)

add_library(i2c
    i2c.cpp
    command-handlers-i2c.cpp
    )

target_link_libraries(i2c protocol)

target_include_directories(i2c INTERFACE include)
