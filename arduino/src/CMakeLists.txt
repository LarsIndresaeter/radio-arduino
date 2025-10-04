add_executable(
    ${PRODUCT_NAME} 
    main.cpp
    )

if(REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE)
    add_definitions(-DREPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE=1)
endif()

target_link_libraries(${PRODUCT_NAME} spi nrf24l01 radio_uart random ssd1306 version ds18b20 i2c ina219 uart protocol quadencoder timer gpio ws2812b sha1 eepromlib aes pwm adc radio_link sleep parser)

include(${CMAKE_SOURCE_DIR}/cmake/avr-macros.cmake)
