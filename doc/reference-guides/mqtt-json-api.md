# mqtt json api

## raduino-mqtt-adapter

These commands must follow the [adapter schema](../../linux/schema/adapter.schema.json)

### resentBirthCertificate

This command will resent birth certificates.
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

This command ust be sent to the `raduino-adapter/RCMD/<id>` topic.

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

## subscription

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

