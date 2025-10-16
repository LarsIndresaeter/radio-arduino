include_directories(include)

add_library(radio-uart
    radioUart.cpp
    )

target_link_libraries(radio-uart com-bus nrf24l01)

target_include_directories(radio-uart INTERFACE include)
