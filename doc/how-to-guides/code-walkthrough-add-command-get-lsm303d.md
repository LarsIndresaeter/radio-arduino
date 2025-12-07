# code walkthrough add new command get_lsm303d

A typical task is to add support for a new sensor.
In this example we walk through the code changes for adding the command get_lsm303d.
The steps to support a new command will typically look like this:

1. Describe requirements
2. Read registers using the tool `raduino-i2c-bridge`
3. Define new command
4. Generate header files for new command
5. Implement command handler for arduino
6. Add new linux command line tool
7. Pubish lsm303d sensor data on mqtt

## requirements

The sensor lsm303d has an accelerometer and a magnetometer.

> **use case:**\
> \
> Read the accelerometer from lsm303d

## Read registers using the tool raduino-i2c-bridge

Check that we can talk to arduino

```console
./bin/raduino-system-commands -p
```

From the datasheet we find that:

- The 8-bit address is **0x32** _(50 decimal)_
- Write the value **0x17** _(23 decimal))_ to register ctrl1 to enable the accelerometer
- The register ctrl1 has the address **0x20** _(32 decimal)_
- The register OUT_X_H_A has the register address **0x29** _(41 decimal)_

### enable accelerometer

```console
./bin/raduino-i2c-bridge -a 50 -c 32 -w 23
```

### read accelerometer

Read high register for x axis

```console
./bin/raduino-i2c-bridge -a 50 -r 4 -c 41
```

