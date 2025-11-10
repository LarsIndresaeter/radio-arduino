# raduino-node

## help menu

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

## commands used

- [set_radio_role](../commands/set_radio_role.md)
- [soft_reset](../commands/soft_reset.md)
- []()

## description 

This app allows you to change the radio role from `node` to `gateway` or change the address of a `node`

