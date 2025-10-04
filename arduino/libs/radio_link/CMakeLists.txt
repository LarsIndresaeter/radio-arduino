include_directories(include)

add_library(radio_link
    radio_link.cpp
    )

target_link_libraries(radio_link nrf24l01 quadencoder)

target_include_directories(radio_link INTERFACE include)
