include_directories(include)

add_library(watchdog
    watchdog.cpp
    )

target_include_directories(watchdog INTERFACE include)
