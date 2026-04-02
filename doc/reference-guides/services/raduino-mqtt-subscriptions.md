# raduino-mqtt-subscriptions

## description

This service communicate with [raduino-mqtt-bridge](../apps/raduino-mqtt-bridge.md)
through [raduino-mqtt-proxy](./raduino-mqtt-proxy.md).
This service will set up subscriptions that poll status from nodes advertised on
the topic `raduino-bridge/ADVERTISEMENT/id`.
Communication with the bridge is done in the name space `raduino-bridge`
while the results are published in the namespace `raduino-subscriptions`.
The service can be polled for status and latest received response from
nodes by sending commands to `raduino-subscription/RCMD`.
The command `raduino mqtt subscriptions` in the [raduino-toolkit](../../explanation/raduino-toolkit.md)
can be used to interact with the service.

This service is in early development and is likely to change.

## requirements

### subscriptions

A key component of this service is the module responsible for subscriptions.
The following list of requirements were collected before developing this service.

- Subscriptions are generic, ie. they behave the same way for all commands.
- Subscription are only used for commands that poll state from a node or gateway.
- A subscription in initialized with the name of the command and the interval.
- Only the last valid result is stored
- All subscriptions for one node is executed before moving to the next node.
- If one command fails on one node then the polling moves on to the next node to avoid blocking.
- the raduino-mqtt-proxy is used to select the best gateway for each node.

