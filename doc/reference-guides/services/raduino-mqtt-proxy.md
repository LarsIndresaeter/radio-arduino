# raduino-mqtt-proxy

## description

This program is intended to run as a service. It will forward mqtt messages sent
to the topic `raduino/RCMD/proxy/node-serial` to the gateway with the highest
quality score for that node with serial number `node-serial`.

