include_directories(include)

add_library(uart
    uart.cpp
    )

target_link_libraries(uart com_bus)

target_include_directories(uart INTERFACE include)
