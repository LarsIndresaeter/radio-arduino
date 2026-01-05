# project

## known issues

The arduino code has only 2k of RAM and can crash if too large objects are
placed on the stack. Be aware of strange behavior when adding new functionality
that place large objects on the stack.

## motivation

This repository is the result of several learning projects which has influenced
the outcome. I have used this to learn:

- [conan](./conan.md)
- cmake
- modern C++
- [template meta programming](../../linux/libs/monitor/include/monitor.hpp)
- custom [binary protocol](./interface/binary-protocol-interface.md) with
  support for encrypted payloads
- OpenSSl 3.1
- [Radio communication](./arduino/radio-communication.md) using the nrf24l01
- [mqtt](./linux/mqtt.md)
- writing better documentation

## raduino name

raduino is simply the short version of the name radio-arduino. The name
radio-arduino was selected because it is a radio project for aduino. The only
supported radio chip is nrf24l01, but in the future it is possible that other
chips like sx1276 (a popular LoRa chip) can be used.

> short catchy and faster to type

### where is the name used and what does it mean

This project is called radio-arduino but sometimes the short form raduino is used.

| where           | meaning                                                     |
| --------------- | ----------------------------------------------------------- |
| conan packages  | Package `raduino-api` is short for `radio-arduino-api`      |
| command classes | `RaduinoCommandPing` shorter than `RadioArduinoCommandPing` |
| toolkit         | The command `raduino` is shorter than `radio-arduino`       |
| apps            | `raduino-node` is shorter than `radio-arduino-node`         |

## folder structure

Explanations follow the folder structure of the project with arduino, interface,
linux and tools as the main folders.

| folder    | description                                             |
| --------- | ------------------------------------------------------- |
| arduino   | source code for atmega328p                              |
| interface | API interface between arduino and linux                 |
| linux     | source code for linux host applications                 |
| tools     | toolkit for working with the code in this repository    |
| doc       | documentation folder                                    |
| build     | build area for arduino, interface and linux source code |
| bin       | compiled binary files                                   |

## how to contribute

If you want to contribute to this code the preffered way is to:

1. clone this repository
2. make the suggested changed to the code
3. create a merge request towards this repository

