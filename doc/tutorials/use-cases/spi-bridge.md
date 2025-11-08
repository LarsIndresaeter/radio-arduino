# spi bridge

generic tool. useful for test and developement 

## write and read example

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

