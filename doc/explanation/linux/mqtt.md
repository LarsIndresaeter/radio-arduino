# mqtt

the linux cli tool `raduino-mqtt-client` is a `mqtt client` that connects to a `mqtt broker`. The client will read data from a `gateway` and `node` and publish it to the `mqtt broker`. The client will subscribe to a topic on the broker and execute commands.

The best way to experiment with this is to set up a mqtt broker on localhost and use the command `raduino mqtt` which will print a help menu

```console
raduino mqtt
commands:
    interval <n> : set polling interval to <n> seconds
     text <text> : write <text> on lcd display
      subscribe  : subscribe to radio-arduino topic
```

The topic names are inspired by the sparkplugB standard but the payload is json (and not protobuf).

## messages published by radiuino-mqtt

```console
radio-arduino/NBIRTH/gateway {"dateString: "2025-11-08 11:36:03"}
radio-arduino/NBIRTH/lcd {"dateString: "2025-11-08 11:36:06"}
radio-arduino/NDATA/lcd/vcc {"name":"vcc", "timestamp":1762601767008, "vcc":4624}
radio-arduino/NDATA/lcd/gpio {"name":"gpio", "timestamp":1762601767024, "portB":15, "portC":48, "portD":3}
```
