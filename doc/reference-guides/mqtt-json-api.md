# mqtt json api

## raduino-mqtt-adapter

These commands must follow the [adapter schema](../../linux/schema/adapter.schema.json).
Parse errors are printer to standard output (console) by the service raduino-mqtt-adapter,
in the future this may be published to a log topic or written to a log file.

### resentBirthCertificate

This command will resend birth certificates.
This command will be picked up by all mqtt adapters.
This is useful for building a list of detected nodes when a new client is started.

The command must be sent to the topic `raduino-adapter/RDMD` or the `raduino-router/RCMD` topic.

```json
{
  "command": "resendBirthCertificate",
  "params": {
    "timestamp": 1778767573998
  }
}
```

### commandList

Command list is a list of the command names to run.
Only commands without paramters can be executed this way.

This command ust be sent to the `raduino-adapter/RCMD/<gw-id>/<id>`
or `raduino-router/RCMD/<id>` topic.

```json
{
  "command": "batchCommand",
  "params": {
    "commandList": [
      "get_device_name",
      "get_statistics",
      "get_version",
      "get_attached_devices_csv_string"
    ],
    "nodeAddress": 1765235324,
    "timestamp": 1778767621549
  }
}
```

### subscription

The subscription command will instuct the node to publish measurements in the advertisement packages.
The subscriptionId is the integer value of the command found in the [commands/README.md](./commands/README.md).
The subscriptionInterval is multiples of [advertisement interval](./commands/set_advertisement_interval.md).

This command ust be sent to the `raduino-adapter/RCMD/<id>` topic.

```json
{
  "command": "subscription",
  "params": {
    "nodeAddress": 1778266869,
    "subscriptionId": 28,
    "subscriptionInterval": 1
  }
}
```

## virtual-device

status is published on the topic `raduino-device/status` by the service raduino-virtual-device

```json
{
  "1765235324": {
    "deviceName": "gw4",
    "nodeType": "gateway",
    "stats": {
      "commandsParsed": 62363,
      "radioRx": 2639674,
      "radioTx": 61922,
      "restarts": 8467,
      "uartRx": 16749454,
      "uartTx": 36597519
    },
    "timestamp": 1778776687868,
    "version": "3.0.0.16+fc9b544"
  },
  "1778266869": {
    "accelerometer": {
      "x": -0.011712,
      "y": -0.050752,
      "z": 1.069696
    },
    "batteryVoltage": 4.555,
    "deviceName": "accelerometer",
    "gpio": {
      "portB": 14,
      "portC": 48,
      "portD": 3
    },
    "nodeType": "node",
    "stats": {
      "commandsParsed": 2120,
      "radioRx": 50653,
      "radioTx": 68446,
      "restarts": 70,
      "uartRx": 0,
      "uartTx": 0
    },
    "timestamp": 1778776699675,
    "version": "3.0.0.16+fc9b544"
  }
}
```

## command response

command response is published on the topic `raduino-adapter/DDATA/<gw-id>/<id>/<command-name>`
by the service raduino-mqtt-adapter and republished on the topic
`raduino-router/DDATA/<id>/<command-name>` by the service raduino-mqtt-router.

json formatting is done by the [generated command classes](../explanation/interface/protocol-command-generator.md).
Either as a response to a command or from the payload of an advertisement.

```json
{
  "name": "get_statistics",
  "timestamp": 1778776699657,
  "responsetimeUs": 18285,
  "responseCode": "success",
  "payload": {
    "commandsParsed": 2120,
    "uartRx": 0,
    "uartTx": 0,
    "radioRx": 50653,
    "radioTx": 68446,
    "restarts": 70
  }
}
```

## advertisements

Advertisements received by the service raduino-mqtt-adapter are published on
the topic `raduino-adapter/ADVERTISEMENT/<gw-id>/<id>` and republished by
the raduino-mqtt-router on the topic `raduino-router/ADVERTISEMENT/<id>`.

```json
{
  "lastAdvertisement": 1778776662570,
  "nodeAddress": 1765235324,
  "nodeType": "gateway",
  "sequenceNumber": 22,
  "subscriptionId": 0
}
```

## birth certificate

Birth certificates are published by the raduino-mqtt-adapter service on the
topic `raduino-adapter/DBIRTH/<gw-id>/<id>` and republished by the service
`raduino-router/DBIRTH/<id>`.

The birth certificate contains the nodeAddress of the radio node,
the address of the gateway, a health indicator for the link between the
gateway and node, time of last advertisement and a list of the last valid
response for each command.

The birth certificate is used by the raduino-router service to detect
the best path for a radio node.
The command responses can be used to rebuild state for micro services consuming command responses.

```json
{
  "gateway": 1765235324,
  "healthIndicator": 1001,
  "lastAdvertisement": 1778776678719,
  "nodeAddress": 1778266869,
  "validResponses": {
    "get_attached_devices_csv_string": {
      "name": "get_attached_devices_csv_string",
      "payload": {
        "csvString": "lsm303d"
      },
      "responseCode": "success",
      "responsetimeUs": 9350,
      "timestamp": 1778776699683
    },
    "get_device_name": {
      "name": "get_device_name",
      "payload": {
        "nameString": "accelerometer"
      },
      "responseCode": "success",
      "responsetimeUs": 11584,
      "timestamp": 1778776694422
    },
    "get_lsm303d": {
      "name": "get_lsm303d",
      "payload": {
        "accelerometerX": 65344,
        "accelerometerY": 64704,
        "accelerometerZ": 17536
      },
      "responseCode": "success",
      "responsetimeUs": 17362,
      "timestamp": 1778776642348
    },
    "get_statistics": {
      "name": "get_statistics",
      "payload": {
        "commandsParsed": 2120,
        "radioRx": 50653,
        "radioTx": 68446,
        "restarts": 70,
        "uartRx": 0,
        "uartTx": 0
      },
      "responseCode": "success",
      "responsetimeUs": 18285,
      "timestamp": 1778776699657
    },
    "get_version": {
      "name": "get_version",
      "payload": {
        "versionString": "3.0.0.16+fc9b544"
      },
      "responseCode": "success",
      "responsetimeUs": 8394,
      "timestamp": 1778776699675
    },
    "gpio": {
      "name": "gpio",
      "payload": {
        "portB": 14,
        "portC": 48,
        "portD": 3
      },
      "responseCode": "success",
      "responsetimeUs": 6410,
      "timestamp": 1778776694416
    },
    "set_subscription": {
      "name": "set_subscription",
      "payload": {},
      "responseCode": "success",
      "responsetimeUs": 7503,
      "timestamp": 1778776694331
    },
    "vcc": {
      "name": "vcc",
      "payload": {
        "vcc": 4555
      },
      "responseCode": "success",
      "responsetimeUs": 15854,
      "timestamp": 1778776694400
    }
  }
}
```

