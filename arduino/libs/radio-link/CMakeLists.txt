include_directories(include)

add_library(radio-link
    radio-link.cpp
    command-handlers-radio-link.cpp
    )

target_link_libraries(radio-link nrf24l01 quadencoder protocol)

target_include_directories(radio-link INTERFACE include)
