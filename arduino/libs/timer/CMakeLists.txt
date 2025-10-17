include_directories(include)

add_library(timer
    timer.cpp
    command-handlers-timer.cpp
    )

target_link_libraries(timer protocol)

target_include_directories(timer INTERFACE include)
