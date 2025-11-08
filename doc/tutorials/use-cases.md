# use cases

The primary use case for the code in this project is the [gateway and node](./gateway-and-node.md) use-case but other use-cases are also supported.

* [ina219 power monitor](./arduino-nano-ina219-power-monitor.md) : read voltage and current and report min, max and average per second
* [nrf24l01 sniffer](./nrf24l01-sniffer.md) : configure the nrf24l01 chip to be in an undocumented verbose mode and send data to the serial port
* [device ina219](./device-ina219.md) : read current and voltage from an ina219 power monitor chip connected to a gateway or node
* [quadrature encoder](./quadrature-encoder.md) : read quadrature encoder using the RaduinoCommandQuadratureEncoder command
* [rs232 radio bridge](./rs232-radio-bridge.md) : configure two gateways to bridge data from rs232 to to the serial port. One device is the sender but communication is two-way


## Gateway and node

This is the default use case. Commands can be sent to either the gateway or the node. Cli tools for debugging or demo and mqtt client

### communication from PC to arduino

```
    ┌───────────┐                 ┌─────────────────┐                ┌─────────────────┐
    │           │                 │                 │                │                 │
    │           │     RS 232      │ ARDUINO NANO    │     RADIO      │ ARDUINO NANO    │
    │    PC     │◄───────────────►│                 │◄──────────────►│                 │
    │           │                 │ WITH NRF24L01+  │                │ WITH NRF24L01+  │
    │           │                 │                 │                │                 │
    └───────────┘                 └─────────────────┘                └─────────────────┘
                                       gateway                              node
```

## rs232 radio bridge

By configuring two rf-nano boards as a gateway and sending RaduinoCommanRadioUart commands you can configure the two boards to create a serial port bridge between two computers.

### configure the two gateways

computer 1 - sender

```console
./bin/raduino-gateway -U s
```

computer 2 - receiver

```console
./bin/raduino-gateway -U r
```

open another serial program and connect to the serial port for the gateway. 

```console
minicom -d /dev/ttyUSB0
```

## spi bridge

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

## i2c bridge

generic tool. useful for test and developement. You will typically use this to experiment with a new i2c device before adding a new command specifically for that device.

### Example

read register 0 from ina219

* ina219 address = 0x80
* default value of register 0, configuration = 0x399F

```console
./bin/raduino-i2c-bridge -d 128 -o 5 -r 2
```

