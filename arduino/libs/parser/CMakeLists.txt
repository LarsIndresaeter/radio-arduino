include_directories(include)

add_library(parser
    parser.cpp
    )

if(REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE)
    add_definitions(-DREPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE=1)
endif()

target_link_libraries(parser aes nrf24l01 random sleep protocol radio_link)

target_include_directories(parser PUBLIC ${CONAN_INCLUDE_DIRS_RADUINO-API})
target_include_directories(parser INTERFACE include)
