include_directories(include)

add_library(gpio
    gpio.cpp
    )

target_include_directories(gpio INTERFACE include)
