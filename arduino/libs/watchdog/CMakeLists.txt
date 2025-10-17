include_directories(include)

add_library(watchdog
    watchdog.cpp
    command-handlers-watchdog.cpp
    )

target_link_libraries(watchdog protocol)

target_include_directories(watchdog INTERFACE include)
