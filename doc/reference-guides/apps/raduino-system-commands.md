# raduino-system-commands

## description

This app lets you do system specific tasks, ie. tasks that are not interacting
with any chip.

## help menu

```console
raduino-system-commands
           -K <key> : encrypt command with transport key
       -N <address> : wakeup node address
                 -a : check if arduino version is correct on node
                 -A : check if arduino version is correct on gateway
                 -b : return avtive and sleep time from gateway
                 -B : return avtive and sleep time from node
                 -c : SHA1 command on gateway
                 -C : SHA1 command on node
                 -d : debug command on gateway
                 -D : debug command on node
                 -e : scan for advertisement
            -E <id> : scan for advertisement from <id>
                 -p : ping gateway
                 -P : ping node
           -f <key> : set data encryption key on gateway
           -F <key> : set data encryption key on node
           -g <key> : set transport encryption key on gateway
           -G <key> : set transport encryption key on node
                 -i : get unique id from gateway
                 -I : get unique id from node
                 -r : get random bytes command on gateway
                 -R : get random bytes command on node
                 -l : get attached devices csv string from gateway
                 -L : get attached devices csv string from node
                 -s : get statistics from gateway
                 -S : get statistics from node
                 -m : get device name from gateway
                 -M : get device name from node
          -t <flag> : set transport encryption required (command must be encrypted) on gateway <0|1>
          -T <flag> : set transport encryption required (command must be encrypted) on node <0|1>
                 -u : unencrypted session (command must be encrypted)
                 -v : get device version from gateway
                 -V : get device version from node
                 -x : HOTP command on gateway
                 -X : HOTP command on node
                 -Z : Verbose on
                 -z : Verbose off
                 -k : set keep alive time in milliseconds
                 -h : print this text
```

## commands used

- [sha1](../commands/sha1.md)
- [set_key](../commands/set_key.md)
- [get_device_name](../commands/get_device_name.md)
- [hotp](../commands/hotp.md)
- [random](../commands/random.md)
- [get_unique_id](../commands/get_unique_id.md)
- [debug](../commands/debug.md)
- [ping](../commands/ping.md)
- [require_transport_encryption](../commands/require_transport_encryption.md)
- [unlock_session](../commands/unlock_session.md)
- [get_statistics](../commands/get_statistics.md)

