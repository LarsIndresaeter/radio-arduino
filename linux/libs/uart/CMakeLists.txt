
include_directories(include)
add_library(uart STATIC uart.cpp)
target_link_libraries(uart epollevent)
target_include_directories(uart INTERFACE include)
