include_directories(include)

add_library(radio_uart
    radioUart.cpp
    )

target_link_libraries(radio_uart com_bus nrf24l01)

target_include_directories(radio_uart INTERFACE include)
