# code walkthrough add new command get_lsm303/

A typical task is to add support for a new sensor.
In this example we walk through the code changes for adding the command get_lsm303d.
The steps to support a new command will typically look like this:

1. Describe requirements
2. Read i2c registers on the device using the tool `raduino-i2c-bridge`
3. Define new command in generator script
4. Generate C++ files for new command
5. Implement command handler for arduino
6. Add new linux command line tool
7. Pubish lsm303d sensor data on mqtt

commits in git history for this code change

```console
git log --graph --oneline --decorate d0c6b79..49093c4
* 49093c4 (HEAD -> main) Merge branch 'add-command-read-stm303d'
* 394aa1f docs(lsm303d): update code walk throug
* fca3c9f feat(lsm303d): publish accelerometer data on mqtt
* 9802211 feat(lsm303d): add linux command line tool
* d5c7737 feat(lsm303d): implement command on arduino
* b01c76c feat(lsm303d): add generated files for new command
* 93992b7 feat(lsm303d): define new command get_lsm303d
* 8952dc2 docs(lsm303d): describe testing in code walkthrough
* 578a6d8 docs(lsm303d): Add requirements to code walkthrough
```

## requirements

The sensor lsm303d has an accelerometer and a magnetometer.

- Read the accelerometer I2C registers on lsm303d
- Add command line interface for reading lsm303d
- publish accelerometer data on mqtt

## Read I2C registers using the tool raduino-i2c-bridge

Check that we can talk to arduino

```console
./bin/raduino-system-commands -p
```

From the lsm303d datasheet we find that:

- The 8-bit address is **0x32** _(50 decimal)_
- Write the value **0x17** _(23 decimal))_ to register ctrl1 to enable the accelerometer
- The register ctrl1 has the address **0x20** _(32 decimal)_
- The register OUT_X_H_A has the register address **0x29** _(41 decimal)_

### enable accelerometer

write to the I2C registers using the new app added in commit `9802211`.

```console
./bin/raduino-i2c-bridge -a 50 -c 32 -w 23
```

### read accelerometer

Read high register for x axis

```console
./bin/raduino-i2c-bridge -a 50 -r 4 -c 41
```

## read accelerometer with cli tool

with the new command line tool you can now read lsm303d connected to a gateway
or node.

read from gateway

```console
./bin/raduino-device-lsm303d -r
```

read from node. Note that `-N 0` will wake up radio node with address 0 in the
implied addressing scheme between radio-arduino gateway and node.

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

start mqtt client which was updated in commit `fca3c9f`.

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

