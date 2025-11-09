# raduino-node

the application [raduino-node](../reference-guides/apps/raduino-node.md) is used for communication with the node device.

a typical first command is to wake up node with address 0, ping and request the node name

```console
./bin/raduino-node -n 0 -p -z
```

if you have enable encryption of the binary packages during the personalization step then you need to add some options

```console
./bin/raduino-node -n 0 -b "secret key" -T -p -z
```

