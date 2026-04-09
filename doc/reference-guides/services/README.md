# linux services

Services are linux applications inteded to run in container environments.
Services run without a raduino device connected.
These services communicate using mqtt and possibly in the future other protocols.

## services

These apps are your main apps

| app                                                           | description                                               |
| ------------------------------------------------------------- | --------------------------------------------------------- |
| [raduino-mqtt-router](./raduino-mqtt-router.md)               | mqtt router for raduino gateways                          |
| [raduino-mqtt-subscriptions](./raduino-mqtt-subscriptions.md) | manage subscriptions to nodes advertised to a mqtt broker |
| [raduino-virtual-device](./raduino-virtual-device.md)         | create a digital shadow of the devices                    |

## setup


### radio node, gateway and adapter software

This figure shows one node communicating with a gateway (both rf-nano boards).
The gateway is connected to a host computer through a serial port (rs232)
while the node and the gateway communicate over a radio link.

```
┌──────────┐       ┌────────────┐         ┌───────────┐
│          │ radio │            │ serial  │           │
│  node 1  │-------│  gateway 1 ├─────────┤  adapter  │
│          │  |    │            │    |    │           │
└──────────┘  |    └────────────┘    |    └───────────┘
              |                      |                 
              |                      |                 
 radio node   |     serial device    |    host computer
              |                      |                 
```

### raduino devices and mqtt broker

This figure shows two nodes communicating with two gateways each.
The gateway is controlled by an adapter which bridges
the raduino protocol with json commands over mqtt.

```
┌──────────┐       ┌────────────┐         ┌───────────┐                                       ┌───────────────┐
│          │       │            │         │           │────[raduino-adapter/gw1/DDATA/id]────►│               │
│  node 1  │-------│  gateway 1 ├─────────┤  adapter  │◄───[raduino-adapter/gw1/RCMD/id]──────┤               │
│          │       │            │         │           │                                       │               │
└──────────┘       └────────────┘         └───────────┘                                       │               │
            \     /                                                                           │               │
             \   /                                                                            │               │
              \ /           ▼                                                                 │               │
               X                                                                              │  mqtt broker  │
              / \                                                                             │               │
             /   \                                                                            │               │
            /     \                                                                           │               │
┌──────────┐       ┌─────────────┐        ┌───────────┐                                       │               │
│          │       │             │        │           ├────[raduino-adapter/gw2/DDATA/id]────►│               │
│  node 2  │-------│  gateway 2  ├────────┤  adapter  │◄───[raduino-adapter/gw2/RCMD/id]──────│               │
│          │       │             │        │           │                                       │               │
└──────────┘       └─────────────┘        └───────────┘                                       └───────────────┘
```

### services

This figure shows shows the topic names used commands and responses between services.
The service `bin/raduino-mqtt-subscriptions` interact with a
[message router](https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageRouter.html).
The router copy the payload to new topics without modifyng the payload.
The `raduino-virtual-device` subscribe to data from nodes and gateways and use
this data to build a digital shadow of the physical devices.

```bash
┌──────────┐                                                                                                
│          │                                   ┌──────────┐                                                 
│ raduino- │◄──[raduino-adapter/RCMD/gw1/id]───┤          │                                                 
│ mqtt-    │                                   │          │                                                 
│ adapter  ├───[raduino-adapter/DDATA/gw1/id]─►│          │                                ┌───────────────┐
│          │                                   │          │                                │               │
└──────────┘                                   │ raduino- │◄───[raduino-router/RCMD/id]────┤ raduino-      │
                                               │ mqtt-    │                                │ mqtt-         │
                                               │ router   ├──┬─[raduino-router/DDATA/id]──►│ subscriptions │
┌──────────┐                                   │          │  │                             │               │
│          │◄──[raduino-adapter/RCMD/gw2/id]───┤          │  │                             └───────────────┘
│ raduino- │                                   │          │  │                                              
│ mqtt-    ├───[raduino-adapter/DDATA/gw2/id]─►│          │  │                                              
│ adapter  │                                   │          │  │                                              
│          │                                   └──────────┘  │                                              
└──────────┘                                                 │                                              
                                                             │                                              
                                                             │                             ┌───────────────┐
                                                             │                             │               │
                                                             └────────────────────────────►┤ raduino-      │
                                                                                           │ virtual-      │
                                                                                           │ device        │
                                                                                           │               │
                                                                                           └───────────────┘
```

### list of mqtt topics

| topic                 | description                                                 |
| --------------------- | ----------------------------------------------------------- |
| raduino-adapter       | Command and response for a gateway                          |
| raduino-device        | publish one json file of the digital shadow                 |
| raduino-log           | log messages                                                |
| raduino-router        | service that route command and response to the best gateway |
| raduino-subscriptions | sets up and handle subscriptions for devices                |

### what you might see in an mqtt client

The following output is grabbed from [mqttui](https://github.com/EdJoPaTo/mqttui),
a tui mqtt client.

```bash
────────Topics (23, 408 messages)─────────╮───────────────────────────────JSON Payload (Bytes: 734)───────────────────────────────╮
▶ raduino-adapter (11 topics, 198 messages│▶ 1765235324                                                                           │
▼ raduino-device (1 topics, 15 messages)  │▼ 1765313195                                                                           │
    status = {"1765235324":{"deviceName":"│  ▼ accelerometer                                                                      │
▶ raduino-log (1 topics, 13 messages)     │      x: -0.978515625                                                                  │
▶ raduino-router (8 topics, 167 messages) │      y: 0.7890625                                                                     │
▶ raduino-subscription (2 topics, 15 messa│      z: 0.515625                                                                      │
                                          │    batteryVoltage: 4.691                                                              │
                                          │    deviceName: "accelerometer"                                                        │
                                          │  ▶ gpio                                                                               │
                                          │  ▶ stats                                                                              │
                                          │    timestamp: 1775684619418                                                           │
                                          │    version: "2.1.0.27+5f341f7"                                                        │
                                          │▶ 1768692402                                                                           │
                                          │                                                                                       │
                                          │───────────────────────────History (15, every ~9.8 seconds)────────────────────────────╮
                                          │Time         QoS         Value                                                         │
                                          │23:43:05.114 AtMostOnce  -0.9765625                                                    │
                                          │23:43:16.554 AtMostOnce  -0.982421875                                                  │
                                          │23:43:16.724 AtMostOnce  -0.982421875                                                  │
                                          │23:43:27.995 AtMostOnce  -0.978515625                                                  █
                                          │23:43:39.436 AtMostOnce  -0.978515625                                                  █
                                          │─────────────────────────────────────────Graph──────────────────────────────────────────
                                          │-0.974609375│    ⢀⠔⠱⡀                                        ⢀⠎⠑⠢⠤⣀⡀                    
                                          │            │   ⡠⠊  ⠘⢄             ⣀⡠⠔⠒⠒⠒⠒⠒⠒⠒⠤⣀⡀            ⢠⠃     ⠈⠑⠢⡀                 
                                          │            │ ⢠⠊     ⠈⠢⡀       ⡠⠔⠊⠉            ⠈⠉⠒⠤⣀       ⡰⠁         ⠘⢄        ⢀⡠⠤⠤⠤⠤⠤⠤
                                          │            │⠔⠁        ⠑⡄   ⣀⠔⠉                     ⠑⠤⡀   ⡜             ⠑⢄    ⡠⠒⠁      
                                          │-0.982421875│           ⠈⢆⠤⠊                          ⠈⠒⢄⠎               ⠈⠢⡠⠔⠉          
                                          │            └───────────────────────────────────────────────────────────────────────────
                                          │     23:41:22                                                                   23:43:39

```

