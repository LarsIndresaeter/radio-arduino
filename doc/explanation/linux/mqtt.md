# mqtt

communication from PC to MQTT broker
------------------------------------

.. code-block:: sh

    ┌───────────┐                 ┌────────────┐
    │           │                 │            │
    │    PC     │                 │   MQTT     │
    │           │       MQTT      │            │
    │           │◄───────────────►│   BROKER   │
    │           │                 │            │
    │           │                 │            │
    └───────────┘                 └────────────┘

## description 

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

## messages published by raduino-bin/raduino-power-monitor-mqtt

The command

```console
./bin/raduino-power-monitor-mqtt -n 5
```

will read voltage and current from an attached ina219 power monitor chip and publish json formatted messages to a mqtt server on localhost. Below is one of these messages.

```
radio-arduino/DDATA/power-monitor/ina219 {"voltage":5.069762, "current":0.045977, "stddev":0.000164, "messageCounter":5, "timestamp": "2025-11-08 17:33:57", "secondsSinceEpoch":1762623237}
```

