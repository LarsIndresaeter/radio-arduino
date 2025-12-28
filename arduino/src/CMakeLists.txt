add_executable(
    ${PRODUCT_NAME} 
    main.cpp
    )

target_link_libraries(${PRODUCT_NAME} spi nrf24l01 radio-uart random ssd1306 version ds18b20 i2c ina219 uart protocol quadencoder timer gpio ws2812b lsm303d sha1 eepromlib aes pwm adc radio-link sleep parser watchdog command-handlers)

# Add the linker flag to generate the map file
target_link_options(${PRODUCT_NAME} PRIVATE 
    "-Wl,-Map,${CMAKE_CURRENT_BINARY_DIR}/arduino.map"
)

include(${CMAKE_SOURCE_DIR}/cmake/avr-macros.cmake)
