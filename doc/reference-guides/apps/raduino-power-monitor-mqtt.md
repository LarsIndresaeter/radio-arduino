# raduino-power-monitor-mqtt

## description

This app read voltage and current from and ina219 power monitor chip and publish
statistics per second on a topic containing the device name.

## help menu

```console
power-monitor
           -K <key> : encrypt command with transport key
       -N <address> : wakeup node address
       -n <seconds> : ina219 power monitor, stats for <N> seconds
                 -h : print this text
```

## commands used

- [ina219](../commands/ina219.md)
- [get_device_name](../commands/get_device_name.md)

