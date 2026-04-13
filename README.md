# radio-arduino

> **user story:** As a developer I want to read a sensor on an
> [rf-nano](https://ardunic.com/product/RF-Nano-Board-NRF24L01-Wireless-Module)
> arduino board via a radio link and send data to
> an mqtt broker.

## Features

- Command line build tools for arduino and linux using conan and cmake.
- [toolkit](./doc/explanation/raduino-toolkit.md) for common developer commands.
- Binary message protocol interface for serial and radio communication.
- [code generator](./doc/explanation/interface/protocol-command-generator.md)
    for C++ command classes.
- Same binary for gateway and node. Role is configured in eeprom via command line.
- [Command line tools](./doc/reference-guides/apps/README.md) for interacting
  with gateway and node, including mqtt client.
- Use Docker for building the application.
- Receipies for using docker to set up a system in a separate repository
  [docker-compose-radio-arduino](https://github.com/LarsIndresaeter/docker-compose-radio-arduino)
- Services for [multi gateway setup](doc/reference-guides/services/README.md)

## Try it on a rf-nano board

1. Connect an rf-arduino to the usb port.
   `/dev/ttyUSB0` is assumed as the device file.
2. Run dockerapp: `docker run -it --device=/dev/ttyUSB0 ghcr.io/larsindresaeter/radio-arduino:latest bash`.
3. Inside the dockerapp
   1. Flash the rf-nano board: `raduino-flash rf-nano /dev/ttyUSB0`
   2. Blink the led: `./bin/raduino-device-atmega328 -b`
4. If you have two rf-arduino boards
   1. Configure one board as gateway: `./bin/raduino-personalize -n <name> -r gateway`
   2. Configure one board as node: `./bin/raduino-personalize -n <name> -r node`
   3. Blink the led on the node via the gateway: `./bin/raduino-device-atmega328 -B`
5. If you have a mqtt broker on localhost
   1. publish periodic stats like battery voltage with the command `./bin/raduino-mqtt-client`
6. If you have multiple gateways you can poll status from gateways and nodes
   with the service [raduino-mqtt-subscriptions](./doc/reference-guides/services/raduino-mqtt-subscriptions.md).

## Documentation index

- [Tutorials](./doc/tutorials/README.md)
- [Howto guides](./doc/how-to-guides/README.md)
- [Reference guides](./doc/reference-guides/README.md)
- [Explanation](./doc/explanation/README.md)

