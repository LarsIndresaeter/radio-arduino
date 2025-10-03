include_directories(include)

add_library(timer
    timer.cpp
    )

target_include_directories(timer INTERFACE include)
