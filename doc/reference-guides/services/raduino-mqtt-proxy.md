# raduino-mqtt-proxy

## description

This program is intended to run as a service.
It will forward mqtt messages sent to the topic
`raduino-adapter/RCMD/proxy/node-serial` to the gateway with the highest
health score for that node with serial number `node-serial`.

This service is intended to be used as a proxy between [raduino-mqtt-bridge](../apps/raduino-mqtt-bridge.md)
and [raduino-mqtt-subscriptions](./raduino-mqtt-subscriptions.md)

