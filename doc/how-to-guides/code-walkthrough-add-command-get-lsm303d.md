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

## read accelerometer with cli tool

with the new command line tool you can now read lsm303d connected to a gateway or node.

read from gateway

```console
./bin/raduino-device-lsm303d -N 0 -r
```

read from node

```console
./bin/raduino-device-lsm303d -N 0 -R
```

result

```console
accelerometer=[-16320, 256, 128]
```

## test mqtt publishig

start mosquitto and subscribe. Installation of mqtt server is not described here.

```console
mosquitto_sub -t '#' -v
```

start mqtt client

```console
./bin/raduino-mqtt-client
```

mqtt messages received  by mosquitto_sub:

```console
radio-arduino/NBIRTH/raduino-gateway- {"dateString: "2025-12-07 09:57:35"}
radio-arduino/NBIRTH/node-acceleromet {"dateString: "2025-12-07 09:57:40"}
radio-arduino/NDATA/node-acceleromet/vcc {"name":"vcc", "timestamp":1765101460669, "responsetimeUs":16593, "responseCode":"success", "payload":{"vcc":4860}}
radio-arduino/NDATA/node-acceleromet/gpio {"name":"gpio", "timestamp":1765101460685, "responsetimeUs":6647, "responseCode":"success", "payload":{"portB":15, "portC":48, "portD":3}}
radio-arduino/NDATA/node-acceleromet/get_lsm303d {"name":"get_lsm303d", "timestamp":1765101460692, "responsetimeUs":9346, "responseCode":"success", "payload":{"accelerometerX":49152, "accelerometerY":192, "accelerometerZ":65472}}
```

Pretty print of json message from the new command `get_lsm303d`.

```json
{
  "name": "get_lsm303d",
  "timestamp": 1765101460692,
  "responsetimeUs": 9346,
  "responseCode": "success",
  "payload": {
    "accelerometerX": 49152,
    "accelerometerY": 192,
    "accelerometerZ": 65472
  }
}
```

