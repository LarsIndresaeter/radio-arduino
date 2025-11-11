#!/usr/bin/env python3

from libgenerate import raduinocommand

# open files
raduinocommand.openFiles()

# command definitions
raduinocommand.add(2, "blink", [], [])
raduinocommand.add(3, "sha1", ["data[20]"], ["data[20]"])
raduinocommand.add(4, "hotp", [], ["data[20]"])
raduinocommand.add(5, "eeprom_write", ["address[2]", "data"], ["address[2]", "data"])
raduinocommand.add(6, "eeprom_read", ["address[2]"], ["address[2]", "data"])
raduinocommand.add(7, "aes", ["type", "data[16]"], ["type", "data[16]"])
raduinocommand.add(8, "pwm", ["port", "pin", "value"], ["port", "pin", "value"])
raduinocommand.add(9, "random", [], ["data[16]"])
raduinocommand.add(10, "debug", [], ["data[32]"])
raduinocommand.add(11, "gpio", [], ["portB", "portC", "portD"])
raduinocommand.add(12, "i2c_write", ["device", "registerAddress[2]", "length", "data[16]"], [])
raduinocommand.add(13, "i2c_read", ["device", "registerAddress[2]", "length"], ["device", "registerAddress[2]", "length", "data[16]"])
raduinocommand.add(14, "ina219", [], ["current[2]", "voltage[2]"])
raduinocommand.add(15, "ds18b20", [], ["temperature[2]"])
raduinocommand.add(16, "set_key", ["keyId", "keyValue[16]"], [])
raduinocommand.add(17, "set_device_name", ["name[16]"], [])
raduinocommand.add(18, "get_device_name", [], ["nameString[16]"])
raduinocommand.add(19, "ws2812b", ["red[8]", "green[8]", "blue[8]"], [])
raduinocommand.add(20, "ssd1306", ["line", "data[16]"], [])
raduinocommand.add(21, "timer", [], ["pulseWidth[2]"])
raduinocommand.add(22, "spi_read", ["reg", "length"], ["reg", "length", "data[32]"])
raduinocommand.add(23, "spi_write", ["reg", "length", "data[32]"], [])
raduinocommand.add(24, "nrf24l01_init", ["txAddr[5]", "rxAddr[5]", "rfChannel", "gateway"], [])
raduinocommand.add(25, "nrf24l01_read", ["length", "data[32]"], ["length", "data[32]"])
raduinocommand.add(26, "nrf24l01_write", ["length", "data[32]"], ["length", "data[32]"])
raduinocommand.add(27, "radio_uart", ["mode"], [])
raduinocommand.add(28, "vcc", [], ["vcc[2]"])
raduinocommand.add(29, "sleep", ["delay[4]"], [])
raduinocommand.add(30, "wakeup", ["checkAttentionFlag"], ["discovered"])
raduinocommand.add(31, "set_node_address", ["nodeAddress"], [])
raduinocommand.add(32, "keep_alive", ["time"], [])
raduinocommand.add(33, "ping", [], [])
raduinocommand.add(34, "quadrature_encoder", [], ["countnegative[2]", "countpositive[2]", "switchposition", "switchcount[2]"])
raduinocommand.add(35, "get_version", [], ["versionString[32]"])
raduinocommand.add(36, "get_statistics", [], ["commandsParsed[2]", "uartRx[4]", "uartTx[4]", "radioRx[4]", "radioTx[4]", "restarts[2]"])
raduinocommand.add(37, "require_transport_encryption", ["value"], [])
raduinocommand.add(38, "set_radio_role", ["isRadioNode"], [])
raduinocommand.add(39, "soft_reset", [], [])
raduinocommand.add(40, "unlock_session", [], [])
raduinocommand.add(41, "set_unique_id", ["id[4]"], [])
raduinocommand.add(42, "get_unique_id", [], ["id[4]"])

# close files
raduinocommand.closeFiles()

