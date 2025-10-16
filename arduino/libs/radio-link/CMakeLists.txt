include_directories(include)

add_library(radio-link
    radio-link.cpp
    )

target_link_libraries(radio-link nrf24l01 quadencoder)

target_include_directories(radio-link INTERFACE include)
