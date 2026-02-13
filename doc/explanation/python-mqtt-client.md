# python mqtt client

> !NOTE
> This is an experimental feature and this doc can be seen like a design brief
> more than documentation of implemented features.

The python mqtt client interacts with the raduino-mqtt-bridge through a mqtt broker

## command response

Commands are sent on the topic 'radio-arduino/RCMD/gatway-name/node-address'
Response are sent on the topic 'radio-arduino/DDATA/gateway-name/node-address'.

where command is a json command of the format

```json
{
  "name": "vcc",
  "nodeAddress": 1765313195,
  "expirationTime": 1234
}
```

`name` is the name of the command.
The current version does not yet support parameters.
Supported commands are currently vcc, gpio, get_version, get_statistics.

## demo

This demo show usage of multiple gateways and nodes.

- connect two gatways (gw4 and gw5) to the usb ports.
- Start two instances of `raduino-mqtt-bridge`.
- Run the script `tools/python/mqtt.py`
- Program three radio nodes with address 1765313195, 1765835992 and 1769294329.

### start two mqtt bridges

```console
./bin/raduino-mqtt-bridge --device /dev/ttyUSB1
connected to gateway: gw4
subscribe to topic: radio-arduino/RCMD/#
registerRadioNode:1765835992
registerRadioNode:1769294329
```

```console
./bin/raduino-mqtt-bridge --device /dev/ttyUSB0
connected to gateway: gw5
subscribe to topic: radio-arduino/RCMD/#
registerRadioNode:1769294329
registerRadioNode:1765313195
```

### communication observed with mosquitto_sub

```console
radio-arduino/RCMD {"command":"resendBirthCertificate"}
radio-arduino/DBIRTH/gw5/1765313195 {"nodeAddress": 1765313195, "gateway": "gw5", "healthIndicator": 0, "lastSeen": 1770938504776}
radio-arduino/DBIRTH/gw4/1765313195 {"nodeAddress": 1765313195, "gateway": "gw4", "healthIndicator": 3, "lastSeen": 1770938505330}
radio-arduino/RCMD/gw4/1765313195 {"name": "vcc", "nodeAddress": 1765313195, "expirationTime": 1234}
radio-arduino/STATE/gw4/1765313195/command {"name": "vcc", "nodeAddress": 1765313195, "expirationTime": 1234}
radio-arduino/DDATA/gw4/1765313195/response {"name":"vcc", "timestamp":1770938519947, "responsetimeUs":16985, "responseCode":"success", "payload":{"vcc":4591}}
radio-arduino/STATE/gw4/1765313195/command (null)
radio-arduino/DBIRTH/gw4/1765313195 {"nodeAddress": 1765313195, "gateway": "gw4", "healthIndicator": 4, "lastSeen": 1770938515364}
radio-arduino/RCMD/gw4/1765313195 {"name": "vcc", "nodeAddress": 1765313195, "expirationTime": 1234}
radio-arduino/STATE/gw4/1765313195/command {"name": "vcc", "nodeAddress": 1765313195, "expirationTime": 1234}
radio-arduino/DDATA/gw4/1765313195/response {"name":"vcc", "timestamp":1770938541775, "responsetimeUs":16851, "responseCode":"success", "payload":{"vcc":4575}}
radio-arduino/STATE/gw4/1765313195/command (null)
radio-arduino/DBIRTH/gw4/1765313195 {"nodeAddress": 1765313195, "gateway": "gw4", "healthIndicator": 5, "lastSeen": 1770938537025}
```

### Output taken from mqttui

```console
▼ radio-arduino (4 topics, 5 messages)
  ▼ DBIRTH (1 topics, 1 messages)
    ▶ gw4 (1 topics, 1 messages)
  ▼ DDATA (1 topics, 1 messages)
    ▶ gw4 (1 topics, 1 messages)
  ▼ RCMD (1 topics, 1 messages)
    ▼ gw4 (1 topics, 1 messages)
        1765313195 = {"expirationTime":1234,"name":"vcc","nodeAddress":1765313195}
  ▼ STATE (1 topics, 2 messages)
    ▼ gw4 (1 topics, 2 messages)
      ▼ 1765313195 (1 topics, 2 messages)
          command = 
```

