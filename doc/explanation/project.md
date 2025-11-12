# project

## known issues

The arduino code has only 2k of RAM and can crash if too large objects are placed on the stack. Be aware of strange behavior when adding new functionality that place large objects on the stack. 

## what remains

- migrate to conan 2
- make mqtt server address and port configurable
- read HOTP key from eeprom
- read HMAC key from eeprom
- send unique id in discover message

## motivation

This repository is the result of several learning projects which has influenced the outcome. I have used this to learn:

* [conan](./conan.md)
* cmake
* modern C++
* [template meta programming](../../linux/libs/monitor/include/monitor.hpp)
* custom [binary protocol](./interface/binary-protocol-interface.md) with support for encrypted payloads
* OpenSSl 3.1
* [Radio communication](../arduino/radio-communication.md) using the nrf24l01
* [mqtt](./linux/mqtt.md)
* writing better documentation

## raduino name

raduino is simply the short version of the name radio-arduino. The name radio-arduino was selected because it is a radio project for aduino. The only supported radio chip is nrf24l01, but in the future it is possible that other chips like sx1276 (a popular LoRa chip) can be used.

> short catch and faster to type

### where is the name used and what does it mean

| where           | meaning                                                                  |
| --------------- | ------------------------------------------------------------------------ |
| conan packages  | Package `raduino-api` is simply short for `radio-arduino-api`            |
| command classes | The class `RaduinoCommandPing` is shorter than `RadioArduinoCommandPing` |
| toolkit         | The command `raduino` is shorter than `radio-arduino`                    |
| apps            | `raduino-node` is shorter than `radio-arduino-node`                      |

### what will it take for the name to change

- a complaint from the trademark owners of arduino
- I come up with a better name

