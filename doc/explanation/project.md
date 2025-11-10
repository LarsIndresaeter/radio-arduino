# project

## known issues

The arduino code has only 2k of RAM and can crash if too large objects are placed on the stack. Be aware of strange behavior when adding new functionality that place large objects on the stack. 

## what remains

- [ ] read HOTP key from eeprom
- [ ] read HMAC key from eeprom
- [ ] clean up linux uart code
- [ ] migrate to conan 2
- [ ] make mqtt server address and port configurable
- [ ] send unique id in discover message
- [ ] create python libraries for command generation

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

