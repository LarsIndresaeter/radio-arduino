# mqtt-get

## command

The command `raduino mqtt subscription list` obtain a json file from
`raduino-mqtt-subscriptions` and print an overview of the raduino devices.

The command `{"command":"getSubscriptionStatus"}` is sent to the topic
`duino-subscription/RCMD` and a response is returned on the topic
`raduino-subscription/status`.
This is implemented in bash in the script
[mqtt-subscriptions.sh](../../../tools/scripts/mqtt-subscription.sh)

### response

```bash
$ raduino mqtt subscription list`
|         id |             name |  quad |    vcc |      accelerometer |                  version |
| ---------- | ---------------- | ----- | ------ | ------------------ | ------------------------ |
| 1765235324 |            "gw4" |     0 |   null |   null, null, null |       "2.1.0.27+5f341f7" |
| 1765313195 |  "accelerometer" |     0 |   4688 | 64768, 7232, 15808 |       "2.1.0.27+5f341f7" |
| 1768692402 |            "gw5" |     0 |   null |   null, null, null |       "2.1.0.27+5f341f7" |
```

## change polling interval for a subscription

```bash
raduino mqtt subscription interval 1765313195 get_lsm303d 5
```

> if the interval is set to 0 seconds then commands will be sent immediately after
> a response is received. This method will result in approximately 10
> command-response cycles per second for a node over a radio link.

