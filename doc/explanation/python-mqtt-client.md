# python mqtt client

> !NOTE
> This is an experimental feature

The python mqtt client interacts with the raduino-mqtt-bridge through a mqtt broker

## command response

Commands are sent on the topic 'radio-arduino/RCMD/gatway-name/node-address'
Response are sent on the topic 'radio-arduino/DDATA/gateway-name/node-address'.

where command is a json command of the format

## demo

connect two gatways (gw4 and gw5) to the usb ports.
Start two instances of `raduino-mqtt-bridge`. Run the script `tools/python/mqtt.py`
Three radio nodes with address 1765313195, 1765835992 and 1769294329

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

### Output taken from mqttui

```console
▼ radio-arduino (10 topics, 11 messages)
  ▼ DBIRTH (5 topics, 6 messages)
    ▼ gw4 (2 topics, 3 messages)
        1765835992 = {"address":"1765835992","healthScore":2,"lastSeen":1770510875772}
        1769294329 = {"address":"1769294329","healthScore":0,"lastSeen":1770510869761}
    ▼ gw5 (3 topics, 3 messages)
        1765313195 = {"address":"1765313195","healthScore":0,"lastSeen":1770510871809}
        1765835992 = {"address":"1765835992","healthScore":2,"lastSeen":1770510871809}
        1769294329 = {"address":"1769294329","healthScore":2,"lastSeen":1770510871809}
  ▼ DDATA (2 topics, 2 messages)
    ▼ gw4 (1 topics, 1 messages)
      ▼ 1765835992 (1 topics, 1 messages)
          gpio = {"name":"gpio","payload":{"portB":15,"portC":0,"portD":3},"responseCode":"success","responsetimeUs":7442,"timestamp":1770510877792}
    ▼ gw5 (1 topics, 1 messages)
      ▼ 1769294329 (1 topics, 1 messages)
          vcc = {"name":"vcc","payload":{"vcc":4453},"responseCode":"success","responsetimeUs":16849,"timestamp":1770510888941}
  ▼ RCMD = {"command":"resendBirthCertificate"}
    ▼ gw4 (1 topics, 1 messages)
        1765835992 = {"command":"passThrough","device":"node","expirationTime":1234,"gatewayAddress":1234,"name":"gpio","nodeAddress":1765835992}
    ▼ gw5 (1 topics, 1 messages)
        1769294329 = {"command":"passThrough","device":"node","expirationTime":1234,"gatewayAddress":1234,"name":"vcc","nodeAddress":1769294329}
```

