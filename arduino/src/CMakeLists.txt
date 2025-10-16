add_executable(
    ${PRODUCT_NAME} 
    main.cpp
    )

target_link_libraries(${PRODUCT_NAME} spi nrf24l01 radio-uart random ssd1306 version ds18b20 i2c ina219 uart protocol quadencoder timer gpio ws2812b sha1 eepromlib aes pwm adc radio-link sleep parser watchdog)

include(${CMAKE_SOURCE_DIR}/cmake/avr-macros.cmake)
