# raduino-mqtt-router

## description

This program is intended to run as a service.
It will forward mqtt messages sent to the topic
`raduino-router/RCMD/serial` to the gateway with the highest
health score for that node with serial number `serial`.

This service is intended to be used as a router between [raduino-mqtt-adapter](../apps/raduino-mqtt-adapter.md)
and [raduino-mqtt-subscriptions](./raduino-mqtt-subscriptions.md)

