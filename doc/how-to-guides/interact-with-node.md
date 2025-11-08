# raduino-node

the application `raduino-node` is used for communication with the node device.

```console
raduino-node
           -K <key> : encrypt command with transport key
       -N <address> : wakeup node address
                 -C : print counter values
                 -g : reboot node as gateway
       -a <address> : update node address
       -k <interval>: set keep alive interval as 100ms + interval*10ms
                 -h : print this text
```

a typical first command is to wake up node with address 0, ping and request the node name

```console
./bin/raduino-node -n 0 -p -z
```

if you have enable encryption of the binary packages during the personalization step then you need to add some options

```console
./bin/raduino-node -n 0 -b "secret key" -T -p -z
```

