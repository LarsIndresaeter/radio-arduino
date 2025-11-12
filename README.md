# radio-arduino

> **user story:**
> 
> _As a hobbyist I want to connect a sensor to an arduino board, communicate with the arduino via a radio link and send the data to the cloud_

**Supported hardware:** [rf-nano](https://ardunic.com/product/RF-Nano-Board-NRF24L01-Wireless-Module)

## Features

- Command line build [tools](./tools/) for [arduino](./arduino/) and [linux](./linux/) using conan and cmake.
- Binary message protocol [interface](./interface/) for serial and radio communication between.
- Same binary for gateway and node, role is configured in eeprom.
- Command line tools for interacting with gateway and node, including mqtt client.

Read more in the [doc](./doc/README.md) folder.

