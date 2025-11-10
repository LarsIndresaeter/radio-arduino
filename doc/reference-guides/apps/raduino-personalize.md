# raduino-personalize

## help menu

```console
raduino-personalize
        -K <key> : encrypt command with transport key
       -n <name> : set device name
        -t <key> : set transport encryption key
        -e <key> : set data encryption key
         -i <id> : set unique id
              -d : dump eeprom contents
       -r <role> : set radio role <gateway|node>
       -s <flag> : set requireTransportEncyption flag <0|1>
              -h : print this text
```

## commands used

- [eeprom_read](../commands/eeprom_read.md)
- [ping](../commands/ping.md)
- [set_device_name](../commands/set_device_name.md)
- [set_key](../commands/set_key.md)
- [require_transport_encryption](../commands/require_transport_encryption.md)
- [set_unique_id](../commands/set_unique_id.md)

## description 

This app is used to configure a new board by saving settings in eeprom on atmega328.

