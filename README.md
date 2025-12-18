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
- Same binary for gateway and node.
- Role is configured in eeprom via command line.
- [Command line tools](./doc/reference-guides/apps/README.md) for interacting
  with gateway and node, including mqtt client.
- Use Docker for building the application.
- After flashing the rf-nano the [docker-compose-radio-arduino](https://github.com/LarsIndresaeter/docker-compose-radio-arduino)
docker-compose file can be used to subscribe to data and store it in influxdb.

## quick start guide

1. Connect a rf-arduino into the usb port. For the rest of the guide
   `/dev/ttyUSB0` is assumed as the device file.
2. Start dockerapp: `docker run -it --device=/dev/ttyUSB0 radio-arduino:1.6.0 bash`.
   More images on [dockerhub](https://hub.docker.com/r/lars32/radio-arduino).
3. Inside the dockerapp
   1. Flash the rf-nano board: `raduino-flash rf-nano /dev/ttyUSB0`
   2. Blink the led: `./bin/raduino-device-atmega328 -b`
4. If you have two rf-arduino boards
   1. Configure one board as gateway: `./bin/raduino-personalize -n <name> -r gateway`
   2. Configure one board as node: `./bin/raduino-personalize -n <name> -r node`
   3. Blink the led on the node: `./bin/raduino-device-atmega328 -B`
5. If you have a mqtt broker on localhost
   1. publish periodic stats like battery voltage with the command `./bin/raduino-mqtt-client`

## Documentation index

- [Tutorials](./doc/tutorials/README.md)
- [Howto guides](./doc/how-to-guides/README.md)
- [Reference guides](./doc/reference-guides/README.md)
- [Explanation](./doc/explanation/README.md)

