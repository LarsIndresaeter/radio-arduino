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

setup: 2 gateways, `gw4` and `gw5`. 3 nodes, `1765313195`, `1765835992` and `1769294329`.

successful mqtt responses sorted by node

```bash
radio-arduino/DDATA/gw5/1765313195/response {"name":"get_device_name", "timestamp":1773492796319, "responsetimeUs":18090, "responseCode":"success", "payload":{"nameString": "accelerometer"}}
radio-arduino/DDATA/gw5/1765313195/response {"name":"get_statistics", "timestamp":1773492802577, "responsetimeUs":16356, "responseCode":"success", "payload":{"commandsParsed":46, "uartRx":0, "uartTx":0, "radioRx":1900, "radioTx":1610, "restarts":961}}
radio-arduino/DDATA/gw5/1765313195/response {"name":"vcc", "timestamp":1773492834772, "responsetimeUs":16042, "responseCode":"success", "payload":{"vcc":4688}}
radio-arduino/DDATA/gw5/1765313195/response {"name":"gpio", "timestamp":1773492861766, "responsetimeUs":13976, "responseCode":"success", "payload":{"portB":14, "portC":48, "portD":3}}
radio-arduino/DDATA/gw5/1765313195/response {"name":"vcc", "timestamp":1773493448739, "responsetimeUs":16041, "responseCode":"success", "payload":{"vcc":4688}}
radio-arduino/DDATA/gw5/1765313195/response {"name":"vcc", "timestamp":1773494595677, "responsetimeUs":16044, "responseCode":"success", "payload":{"vcc":4688}}
radio-arduino/DDATA/gw5/1765313195/response {"name":"vcc", "timestamp":1773495198206, "responsetimeUs":15678, "responseCode":"success", "payload":{"vcc":4688}}
```

```bash
radio-arduino/DDATA/gw4/1765835992/response {"name":"vcc", "timestamp":1773492757049, "responsetimeUs":21976, "responseCode":"success", "payload":{"vcc":4536}}
radio-arduino/DDATA/gw4/1765835992/response {"name":"gpio", "timestamp":1773492763301, "responsetimeUs":6309, "responseCode":"success", "payload":{"portB":15, "portC":0, "portD":3}}
radio-arduino/DDATA/gw4/1765835992/response {"name":"get_device_name", "timestamp":1773492769538, "responsetimeUs":10360, "responseCode":"success", "payload":{"nameString": "quadencoder"}}
radio-arduino/DDATA/gw4/1765835992/response {"name":"get_statistics", "timestamp":1773492775762, "responsetimeUs":17057, "responseCode":"success", "payload":{"commandsParsed":36, "uartRx":0, "uartTx":0, "radioRx":1128, "radioTx":1248, "restarts":172}}
radio-arduino/DDATA/gw4/1765835992/response {"name":"vcc", "timestamp":1773493365546, "responsetimeUs":15904, "responseCode":"success", "payload":{"vcc":4537}}
radio-arduino/DDATA/gw4/1765835992/response {"name":"vcc", "timestamp":1773493975985, "responsetimeUs":16019, "responseCode":"success", "payload":{"vcc":4547}}
radio-arduino/DDATA/gw4/1765835992/response {"name":"vcc", "timestamp":1773494591569, "responsetimeUs":23671, "responseCode":"success", "payload":{"vcc":4537}}
radio-arduino/DDATA/gw4/1765835992/response {"name":"vcc", "timestamp":1773495196850, "responsetimeUs":15859, "responseCode":"success", "payload":{"vcc":4535}}
```

```bash
radio-arduino/DDATA/gw5/1769294329/response {"name":"vcc", "timestamp":1773492806085, "responsetimeUs":15923, "responseCode":"success", "payload":{"vcc":4703}}
radio-arduino/DDATA/gw5/1769294329/response {"name":"gpio", "timestamp":1773492812327, "responsetimeUs":6513, "responseCode":"success", "payload":{"portB":15, "portC":48, "portD":3}}
radio-arduino/DDATA/gw5/1769294329/response {"name":"get_statistics", "timestamp":1773492823721, "responsetimeUs":10491, "responseCode":"success", "payload":{"commandsParsed":53, "uartRx":0, "uartTx":0, "radioRx":1756, "radioTx":1736, "restarts":140}}
radio-arduino/DDATA/gw5/1769294329/response {"name":"get_device_name", "timestamp":1773492845423, "responsetimeUs":17049, "responseCode":"success", "payload":{"nameString": "lcd"}}
radio-arduino/DDATA/gw5/1769294329/response {"name":"vcc", "timestamp":1773493563824, "responsetimeUs":21109, "responseCode":"success", "payload":{"vcc":4708}}
radio-arduino/DDATA/gw5/1769294329/response {"name":"vcc", "timestamp":1773494173852, "responsetimeUs":24187, "responseCode":"success", "payload":{"vcc":4708}}
radio-arduino/DDATA/gw5/1769294329/response {"name":"vcc", "timestamp":1773495052255, "responsetimeUs":15971, "responseCode":"success", "payload":{"vcc":4708}}
```

