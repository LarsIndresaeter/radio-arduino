include_directories(include)

add_library(ws2812b
    ws2812b.cpp
    command-handlers-ws2812b.cpp
    )

target_link_libraries(ws2812b protocol)

target_include_directories(ws2812b INTERFACE include)
