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

