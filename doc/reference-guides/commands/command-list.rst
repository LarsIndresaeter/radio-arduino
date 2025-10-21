command list
============

arduino
-------

Commands that do something with the avr chip

* blink
* eeprom_write
* eeprom_read
* pwm
* gpio
* i2c_write
* i2c_read
* timer
* spi_read
* spi_write
* vcc
* quadrature_encoder
* soft_reset
* sleep

devices
-------

Commands that talk to a chip on I2C or SPI

* ina219
* ds18b20
* ws2812b
* ssd1306
* nrf24l01_init
* nrf24l01_read
* nrf24l01_write

system
------

commands that interact with the system state

* set_device_name
* get_device_name
* ping
* get_version
* get_statistics
* require_transport_encryption
* sha1
* hotp
* aes
* random
* debug
* set_key
* radio_uart
* wakeup
* set_node_address
* keep_alive
* set_radio_role
* unlock_session

