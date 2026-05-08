# mqtt json api

The API for the `raduino-mqtt-adapter` is summarized in the table below.

| topic                 | description |
| --------------------- | ----------- |
| raduino-adapter/RCMD/ | command     |

## commands

### commandList

```json
{
  "commandList": [
    "get_version",
    "get_attached_devices_csv_string"
  ],
  "nodeAddress": 1765835992,
  "timestamp": 1777485118819
}
```

Command list is a list of the command names to run.
Only commands without paramters can be executed this way.

### resentBirthCertificate

This command will resent birth certificates.
This is useful for building a list of detected nodes when a new client is started.

```json
{                                                                  
  "command": "resendBirthCertificate"                              
}
```

## subscription

The subscription command will instuct the node to publish measurements in the advertisement packages.
The subscriptionId is the integer value of the command found in the [commands/README.md](./commands/README.md).
The subscriptionInterval is multiples of [advertisement interval](./commands/set_advertisement_interval.md).

```json
{                                                                  
  "subscription": {
    "subscriptionId": <uint8_t>,
    "subscriptionInterval": <uint16_t>
  }
  "nodeAddress": 1765835992,
  "timestamp": 1777485118819
}
```
