# configure device

help menu

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

Example 

```console
./bin/raduion-personalize -n DEVICE-NAME -c '' -t 'secret key' -r gateway
```

