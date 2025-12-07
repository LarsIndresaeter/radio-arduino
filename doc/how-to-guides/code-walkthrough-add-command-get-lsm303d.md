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

