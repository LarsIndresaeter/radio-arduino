# raduino-cluster-controller

## description

This service is in early development and is likely to change.

## requirements

### subscriptions

A key component of this service is the module responsible for subscriptions.
The following list of requirements were collected before developing this service.

- [x] Subscriptions are generic, ie. they behave the same way for all commands.
- [x] Subscription are only used for commands that poll state from a node or gateway.
- [x] A subscription in initialized with the name of the command and the interval.
- [x] Only the last valid result is stored
- [x] All subscriptions for one node is executed before moving to the next node.
- [x] If one command fails on one node then the polling moves on to the next node to avoid blocking.
- [x] a state machine is implemented per node since the raduino-mqtt-bride cannot queue messages
- [x] the raduino-mqtt-proxy is used to select the best gateway for each node.

## test

### setup

#### gateways

. id=1765235324, name=gw4
- id=1768692402, name=gw5

#### nodes

- id=1765313195, name=accelerometer
- id=1769294329, name=lcd`

### mqtt response to subscriptions

Subscriptions are setup after birth certificate is received by `raduino-cluster-controller`.
In future version these subscriptions will be configurable.

```bash
radio-arduino/DDATA/1765235324/1765235324/response {"name":"get_device_name", "timestamp":1773682835038, "responsetimeUs":6450, "responseCode":"success", "payload":{"nameString": "gw4"}}
radio-arduino/DDATA/1765235324/1765235324/response {"name":"get_statistics", "timestamp":1773682835152, "responsetimeUs":6617, "responseCode":"success", "payload":{"commandsParsed":22545, "uartRx":226135, "uartTx":316951, "radioRx":19662, "radioTx":1080, "restarts":8105}}
radio-arduino/DDATA/1765235324/1765235324/response {"name":"get_version", "timestamp":1773682835267, "responsetimeUs":3295, "responseCode":"success", "payload":{"versionString": "2.0.1.19+f6237be-dirty"}}
```

```bash
radio-arduino/DDATA/1768692402/1768692402/response {"name":"get_device_name", "timestamp":1773682832706, "responsetimeUs":6591, "responseCode":"success", "payload":{"nameString": "gw5"}}
radio-arduino/DDATA/1768692402/1768692402/response {"name":"get_statistics", "timestamp":1773682832820, "responsetimeUs":6577, "responseCode":"success", "payload":{"commandsParsed":59180, "uartRx":3213935, "uartTx":4502676, "radioRx":34633, "radioTx":740, "restarts":1007}}
radio-arduino/DDATA/1768692402/1768692402/response {"name":"get_version", "timestamp":1773682832935, "responsetimeUs":3196, "responseCode":"success", "payload":{"versionString": "2.0.1.19+f6237be-dirty"}}
```

```bash
radio-arduino/DDATA/1765235324/1765313195/response {"name":"vcc", "timestamp":100087321073246, "responsetimeUs":100087485103488, "responseCode":"error", "payload":{"vcc":0}}
radio-arduino/DDATA/1765235324/1765313195/response {"name":"gpio", "timestamp":1773682832385, "responsetimeUs":14814, "responseCode":"success", "payload":{"portB":14, "portC":48, "portD":3}}
radio-arduino/DDATA/1765235324/1765313195/response {"name":"get_device_name", "timestamp":1773682832518, "responsetimeUs":10467, "responseCode":"success", "payload":{"nameString": "accelerometer"}}
radio-arduino/DDATA/1765235324/1765313195/response {"name":"get_statistics", "timestamp":1773682832646, "responsetimeUs":10569, "responseCode":"success", "payload":{"commandsParsed":10, "uartRx":0, "uartTx":0, "radioRx":164, "radioTx":328, "restarts":961}}
radio-arduino/DDATA/1765235324/1765313195/response {"name":"vcc", "timestamp":1773682849702, "responsetimeUs":24893, "responseCode":"success", "payload":{"vcc":4688}}
radio-arduino/DDATA/1765235324/1765313195/response {"name":"vcc", "timestamp":100087321073246, "responsetimeUs":100087485103488, "responseCode":"error", "payload":{"vcc":0}}
radio-arduino/DDATA/1765235324/1765313195/response {"name":"vcc", "timestamp":100087321073246, "responsetimeUs":100087485103488, "responseCode":"error", "payload":{"vcc":0}}
radio-arduino/DDATA/1765235324/1765313195/response {"name":"vcc", "timestamp":100087321073246, "responsetimeUs":100087485103488, "responseCode":"error", "payload":{"vcc":0}}
radio-arduino/DDATA/1765235324/1765313195/response {"name":"vcc", "timestamp":100087321073246, "responsetimeUs":100087485103488, "responseCode":"error", "payload":{"vcc":0}}
```

```bash
radio-arduino/DDATA/1765235324/1769294329/response {"name":"vcc", "timestamp":1773682834524, "responsetimeUs":24863, "responseCode":"success", "payload":{"vcc":4699}}
radio-arduino/DDATA/1765235324/1769294329/response {"name":"gpio", "timestamp":1773682834669, "responsetimeUs":6215, "responseCode":"success", "payload":{"portB":15, "portC":48, "portD":3}}
radio-arduino/DDATA/1765235324/1769294329/response {"name":"get_device_name", "timestamp":1773682834796, "responsetimeUs":9520, "responseCode":"success", "payload":{"nameString": "lcd"}}
radio-arduino/DDATA/1765235324/1769294329/response {"name":"get_statistics", "timestamp":1773682834923, "responsetimeUs":8646, "responseCode":"success", "payload":{"commandsParsed":44, "uartRx":0, "uartTx":0, "radioRx":1068, "radioTx":1456, "restarts":145}}
radio-arduino/DDATA/1765235324/1769294329/response {"name":"vcc", "timestamp":100087321073246, "responsetimeUs":100087485103488, "responseCode":"error", "payload":{"vcc":0}}
radio-arduino/DDATA/1765235324/1769294329/response {"name":"vcc", "timestamp":100087321073246, "responsetimeUs":100087485113856, "responseCode":"error", "payload":{"vcc":0}}
radio-arduino/DDATA/1765235324/1769294329/response {"name":"vcc", "timestamp":100087321073246, "responsetimeUs":100087485113856, "responseCode":"error", "payload":{"vcc":0}}
radio-arduino/DDATA/1765235324/1769294329/response {"name":"vcc", "timestamp":100087321073246, "responsetimeUs":100087485113856, "responseCode":"error", "payload":{"vcc":0}}
radio-arduino/DDATA/1765235324/1769294329/response {"name":"vcc", "timestamp":100087321073246, "responsetimeUs":100087485113856, "responseCode":"error", "payload":{"vcc":0}}
```

