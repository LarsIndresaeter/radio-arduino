include_directories(include)

add_library(command-handlers
    command-handlers.cpp
    )

target_link_libraries(command-handlers version uart nrf24l01 parser eepromlib protocol)

target_include_directories(command-handlers INTERFACE include)
