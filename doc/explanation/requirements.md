# Requirements

## use cases

> High level interactions between systems

The primary use case for the code in this project is the gateway and node
use-case but other use-cases are also supported.

- [raduino-power-monitor](../reference-guides/apps/raduino-power-monitor-mqtt.md):
read voltage and current and report min, max and average per second
- [raduino-device-ina219](../reference-guides/apps/raduino-device-ina219.md):
read current and voltage from an ina219 power monitor chip connected to a
gateway or node

## Gateway and node

This is the default use case. Commands can be sent to either the gateway or the
node. Cli tools for debugging or demo and mqtt client

### communication from PC to arduino

```console
   ┌───────────┐              ┌─────────────────┐            ┌─────────────────┐
   │           │              │                 │            │                 │
   │           │    RS 232    │ ARDUINO NANO    │    RADIO   │ ARDUINO NANO    │
   │    PC     │◄────────────►│                 │◄──────────►│                 │
   │           │              │ WITH NRF24L01+  │            │ WITH NRF24L01+  │
   │           │              │                 │            │                 │
   └───────────┘              └─────────────────┘            └─────────────────┘
                                   gateway                          node
```

## [spi bridge](../reference-guides/apps/raduino-spi-bridge.md)

generic tool. useful for test and developement

### write and read example

Write 55 to RF_CH register (5) on nrf24l02 on rf-nano

```console
./bin/raduino-spi-bridge -o 5 -w 33
```

Read RF_CH register (5) on nrf24l01 on rf-nano

```console
./bin/raduino-spi-bridge -o 5 -r 1
```

output (note that 33 decimal is 0x21 hex)

```console
SPI_READ               :  reg=5 length=1 data=[ 21 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ]
```

## [i2c bridge](../reference-guides/apps/raduino-i2c-bridge.md)

generic tool. useful for test and developement. You will typically use this to
experiment with a new i2c device before adding a new command specifically for
that device.

### Example

read register 0 from ina219

- ina219 address = 0x80
- default value of register 0, configuration = 0x399F

```console
./bin/raduino-i2c-bridge -d 128 -o 5 -r 2
```

