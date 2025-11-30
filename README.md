# radio-arduino

> **user story:**
> _As a hobbyist I want to connect a sensor to an
> [rf-nano](https://ardunic.com/product/RF-Nano-Board-NRF24L01-Wireless-Module)
> arduino board, communicate with the arduino via a radio link and send data to
> the cloud using [mqtt](./doc/explanation/linux/mqtt.md)._

## Features

- Command line build [tools](./tools/) for [arduino](./arduino/) and
  [linux](./linux/) using conan and cmake including
  [toolkit](./doc/explanation/raduino-toolkit.md) for common developer commands.
- Binary message protocol [interface](./interface/) for serial and radio
  communication with [code generator](./doc/explanation/interface/protocol-command-generator.md).
- Same binary for gateway and node, role is [configured in eeprom](./doc/tutorials/configure-device.md).
- [Command line tools](./doc/reference-guides/apps/README.md) for interacting
  with gateway and node, including mqtt client.

## try it out using docker images from dockerhub

- dockerhub.com/lars32/radio-arduino

Read more in the [doc](./doc/README.md) folder.

