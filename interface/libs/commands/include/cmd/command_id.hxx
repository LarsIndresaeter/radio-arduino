#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

namespace COMMANDS {

constexpr uint8_t MAX_PAYLOAD_LENGTH = 136;
constexpr uint8_t MAX_PACKAGE_LENGTH = (MAX_PAYLOAD_LENGTH + 4 + 2 + 20); // + header, crypto, checksum

enum class OI
{
    UNDEFINED = 0,
    RESERVED = 1,
    BLINK = 2,
    SHA1 = 3,
    HOTP = 4,
    EEPROM_WRITE = 5,
    EEPROM_READ = 6,
    AES = 7,
    PWM = 8,
    RANDOM = 9,
    DEBUG = 10,
    GPIO = 11,
    I2C_WRITE = 12,
    I2C_READ = 13,
    INA219 = 14,
    DS18B20 = 15,
    SET_KEY = 16,
    SET_DEVICE_NAME = 17,
    GET_DEVICE_NAME = 18,
    WS2812B = 19,
    SSD1306 = 20,
    TIMER = 21,
    SPI_READ = 22,
    SPI_WRITE = 23,
    NRF24L01_INIT = 24,
    NRF24L01_READ = 25,
    NRF24L01_WRITE = 26,
    RADIO_UART = 27,
    VCC = 28,
    SLEEP = 29,
    WAKEUP = 30,
    SET_NODE_ADDRESS = 31,
    KEEP_ALIVE = 32,
    PING = 33,
    QUADRATURE_ENCODER = 34,
    GET_VERSION = 35,
    GET_STATISTICS = 36,
    REQUIRE_TRANSPORT_ENCRYPTION = 37,
    SET_RADIO_ROLE = 38,
    SOFT_RESET = 39,
    UNENCRYPTED_SESSION = 40,
};

} // namespace COMMANDS
