# project

## history

History of this project

| year | focus                       |
| ---- | --------------------------- |
| 2022 | conan and binary protocol   |
| 2023 |                             |
| 2023 | mqtt                        |
| 2025 | documentation and usability |

## feature requests

List of requested features with a description:

| feature            | description                                     | plan                                   |
| ------------------ | ----------------------------------------------- | -------------------------------------- |
| uniq id            | add a uniq id for each device                   | likely to be added in upcoming release |
| uniq id            | (gateway and node) with commands to set and get | likely to be added in upcoming release |
| HMAC key in eeprom | read HMAC key from eeprom                       | likely to be added in upcoming release |
| HOTP key in eeprom | read HOTP key from eeprom                       | likely to be added in upcoming release |
| conan 2            | update to conan 2                               | On hold for now                        |
| modbus command     | interact with modbus device on radio node       | under evaluation                       |

## sprint goal

The current sprint goal is to learn how to write better documentation

## previous sprint goals

- learn how to use conan to build for atmega328
- C++ templates
- how to write radio protocols

## known issues

The arduino code has only 2k of RAM and can crash if too large objects are placed on the stack. Be aware of strange behavior when adding new functionality that place large objects on the stack. 

## what remains

- [ ] read HOTP key from eeprom
- [ ] clean up linux uart code
- [ ] migrate to conan 2
- [ ] make mqtt server address and port configurable
- [ ] send unique id in discover message

## motivation

This repository is the result of several learning projects which has influenced the outcome. I have used this to learn:

* [conan](./conan.md)
* cmake
* modern C++
* [template meta programming](../../linux/libs/monitor/include/monitor.hpp)
* custom [binary protocol](./binary-protocol-interface.md) with support for encrypted payloads
* OpenSSl 3.1
* [Radio communication](./radio-communication.md) using the nrf24l01
* [mqtt](./linux/mqtt.md)
* writing better documentation

