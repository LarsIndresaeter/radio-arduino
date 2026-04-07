# raduino-virtual-device

## description

Subscribe to data on topic `raduino-subscription/DDATA/<id>/command`.
Build up a digital shadow of raduino devices, nodes and gateways.
Publish status on the topic `raduino-device/id/status`.
In the future possibly publish desired state on the topic `raduino-device/id/spec`.

## example output

```json
{
  "1765235324": {
    "deviceName": "gw4",
    "timestamp": 1775579917703,
    "version": "2.1.0.27+5f341f7"
  },
  "1768692402": {
    "deviceName": "gw5",
    "timestamp": 1775579921354,
    "version": "2.1.0.27+5f341f7"
  },
  "4276633941": {
    "accelerometer": {
      "x": 178,
      "y": -87,
      "z": 174
    },
    "deviceName": "accelerometer",
    "timestamp": 1775579916179,
    "version": "2.1.0.27+5f341f7"
  }
}
```

