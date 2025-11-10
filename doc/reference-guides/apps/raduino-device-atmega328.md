# raduino-device-atmega328

## help menu

```console
raduino-device-atmega328
           -K <key> : encrypt command with transport key
       -N <address> : wakeup node address
                 -b : Blink command to gateway
                 -B : Blink command to node
         -g <value> : get gpio values on gateway
         -G <value> : get gpio values on node
                 -j : read vcc from gateway
                 -J : read vcc from node
         -p <value> : pwm command to gateway
         -P <value> : pwm command to node
                 -q : read quadrature encoder from gateway
                 -Q : read quadrature encoder from node
                 -C : wake up sleeping rx node if data available flag is set
                 -A : read quadrature encoder from node on change
          -y <name> : set device name from gateway
          -Y <name> : set device name from node
                 -z : get device name from gateway
                 -Z : get device name from node
                 -s : sleep gateway
                 -S : sleep node
                 -t : get pulse width from servo controller 100-200 (1-2 ms) on gateway
                 -T : get pulse width from servo controller 100-200 (1-2 ms) on node
                 -h : print this text
```

## commands used

- [gpio](../commands/gpio.md)
- [vcc](../commands/vcc.md)
- [pwm](../commands/pwm.md)
- [quadrature_encoder](../commands/quadrature_encoder.md)
- [wakeup](../commands/wakeup.md)
- [set_device_name](../commands/set_device_name.md)
- [sleep](../commands/sleep.md)
- [timer](../commands/timer.md)

## description 

This linux interact with the atmega328 chip. Gpio, adc, timer, watchdog and eeprom.

