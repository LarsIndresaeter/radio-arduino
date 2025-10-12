include_directories(include)

add_library(ws2812b
    ws2812b.cpp
    )

target_include_directories(ws2812b INTERFACE include)
