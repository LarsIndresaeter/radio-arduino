# raduino-mqtt-client

## description

This app acts as a mqtt client. It can read status from a radio node and publish
it to a mqtt broker or set a text string on a lcd display published on the
correct topic to the mqtt broker.

## help menu

```console
raduino-mqtt-client
           -K <key> : encrypt command with transport key
       -N <address> : wakeup node address
       -n <address> : gateway address
                 -h : p rint this text
```

## commands used

- [get_device_name](../commands/get_device_name.md)
- [wakeup](../commands/wakeup.md)
- [vcc](../commands/vcc.md)
- [gpio](../commands/gpio.md)
- [quadrature_encoder](../commands/quadrature_encoder.md)
- [ssd1306](../commands/ssd1306.md)

