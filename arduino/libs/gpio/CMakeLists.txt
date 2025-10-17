include_directories(include)

add_library(gpio
    gpio.cpp
    command-handlers-gpio.cpp
    )

target_link_libraries(gpio protocol)

target_include_directories(gpio INTERFACE include)
