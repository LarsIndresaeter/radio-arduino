# radio-arduino

> **user story:**\
> I want to read a sensor on a
> [rf-nano](https://ardunic.com/product/RF-Nano-Board-NRF24L01-Wireless-Module)
> arduino board via a radio link and send data to
> the cloud using [mqtt](./doc/explanation/linux/mqtt.md)

- [more user stories](doc/explanation/requirements.md)

## Features

- Command line [build tools](./tools/) for [arduino](./arduino/) and
  [linux](./linux/) using conan and cmake.
- [toolkit](./doc/explanation/raduino-toolkit.md) for common developer commands.
- Binary message protocol [interface](./interface/) for serial and radio
  communication
- [code generator](./doc/explanation/interface/protocol-command-generator.md)
    for C++ command classes.
- Same binary for gateway and node.
- Role is [configured in eeprom](./doc/tutorials/configure-device.md) via
  command line [tool](./doc/reference-guides/apps/raduino-personalize.md)
- [Command line tools](./doc/reference-guides/apps/README.md) for interacting
  with gateway and node, including mqtt client.
- Use Docker for [building or running](./doc/how-to-guides/howto-use-dockerapp.md)
  the application
- After flashing the rf-nano the [docker-compose-radio-arduino](https://github.com/LarsIndresaeter/docker-compose-radio-arduino)
docker-compose file can be used to subscribe to data and store it in influxdb.

## Documentation index

- [Tutorials](./doc/tutorials/README.md)
- [Howto guides](./doc/how-to-guides/README.md)
- [Reference guides](./doc/reference-guides/README.md)
- [Explanation](./doc/explanation/README.md)

