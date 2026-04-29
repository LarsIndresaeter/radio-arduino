# features

## subscription

### pull based via the `raduino-mqtt-subscriptions` service

The service `raduino-mqtt-subscription` will set up subscriptions based on the
text string read from the radio node with the `get_attached_devices_csv_string` command.

### push based with the `set_subscription` command

The raduino command `set_subscription` will set up a subscription in the raduino radio node.
This subscription will broadcast a message in the discovery package.

