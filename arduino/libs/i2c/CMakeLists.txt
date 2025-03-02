include_directories(include)

add_library(i2c
    i2c.cpp
    )

target_include_directories(i2c INTERFACE include)
