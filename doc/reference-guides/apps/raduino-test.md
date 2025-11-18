# raduino-test

## description

This app is used for testing the if commands return the expected result or just
printing the result.

## help menu

```console
raduino-test
           -K <key> : encrypt command with transport key
       -N <address> : wakeup node address
                 -A : test AES
                 -C : print counter values
                 -e : EEPROM command
                 -I : I2C read command
                 -s : test sha1 command
                 -g : dump eeprom from mega328p
                 -b : test json formatter
                 -S : rx and tx statistics for node and gateway
                 -V : Verbose on
                 -v : Verbose off
                 -h : print this text
```

## commands used

- [aes](../commands/aes)
- [eeprom_read](../commands/eeprom_read.md)
- [eeprom_write](../commands/eeprom_write.md)
- [i2c_read](../commands/i2c_read.md)
- [i2c_write](../commands/i2c_write.md)
- [get_statistics](../commands/get_statistics.md)
- [gpio](../commands/gpio.md)
- [sha1](../commands/sha1.md)
- [hotp](../commands/hotp.md)
- [debug](../commands/debug.md)
- [random](../commands/random.md)
- [get_version](../commands/get_version.md)
- [get_device_name](../commands/get_device_name.md)
- [vcc](../commands/vcc.md)
- [ping](../commands/ping.md)

