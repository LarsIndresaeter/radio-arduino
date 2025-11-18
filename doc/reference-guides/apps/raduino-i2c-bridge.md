# raduino-i2c-bridge

## description

This is a general purpose tool for interacting with i2c devices connected to atmega328.
Typical usage is for experimentation before or during creation of a custom command
that interacts with a device on the i2c bus.

## help menu

```console
raduino-i2c-bridge
               -K <key> : encrypt command with transport key
           -N <address> : wakeup node address
           -a <address> : device address
          -c <register> : control register address
            -r <length> : read <length> bytes
            -s <string> : write <string>
             -w <value> : write <value>
                     -V : Verbose on
                     -h : print this text
```

## commands used

- [i2c_read](../commands/i2c_read.md)
- [i2c_write](../commands/i2c_write.md)

