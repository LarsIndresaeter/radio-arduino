#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

#include <cmd/blink/command.hpp>
#include <cmd/sha1/command.hpp>
#include <cmd/hotp/command.hpp>
#include <cmd/eeprom_write/command.hpp>
#include <cmd/eeprom_read/command.hpp>
#include <cmd/aes/command.hpp>
#include <cmd/pwm/command.hpp>
#include <cmd/random/command.hpp>
#include <cmd/debug/command.hpp>
#include <cmd/gpio/command.hpp>
#include <cmd/i2c_write/command.hpp>
#include <cmd/i2c_read/command.hpp>
#include <cmd/ina219/command.hpp>
#include <cmd/ds18b20/command.hpp>
#include <cmd/set_key/command.hpp>
#include <cmd/set_device_name/command.hpp>
#include <cmd/get_device_name/command.hpp>
#include <cmd/ws2812b/command.hpp>
#include <cmd/ssd1306/command.hpp>
#include <cmd/timer/command.hpp>
#include <cmd/spi_read/command.hpp>
#include <cmd/spi_write/command.hpp>
#include <cmd/nrf24l01_init/command.hpp>
#include <cmd/nrf24l01_read/command.hpp>
#include <cmd/nrf24l01_write/command.hpp>
#include <cmd/radio_uart/command.hpp>
#include <cmd/vcc/command.hpp>
#include <cmd/sleep/command.hpp>
#include <cmd/wakeup/command.hpp>
#include <cmd/set_node_address/command.hpp>
#include <cmd/keep_alive/command.hpp>
#include <cmd/ping/command.hpp>
#include <cmd/quadrature_encoder/command.hpp>
#include <cmd/get_version/command.hpp>
#include <cmd/get_statistics/command.hpp>
#include <cmd/require_transport_encryption/command.hpp>
#include <cmd/set_radio_role/command.hpp>
