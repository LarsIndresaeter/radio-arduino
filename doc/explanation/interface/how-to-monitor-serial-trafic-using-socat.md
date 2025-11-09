# monitor serial traffic

TODO: try this and document how to do it

install socat and set up a bride

```console
socat -x -v /dev/ttyUSB0,rawer,b1000000,crnl PTY,link=$HOME/myserial,rawer,crnl
```

run a command, for example:

```console
./bin/raduino-system-commands --device $HOME/myserial -p
```

which will generate this output in the terminal where you started socat

```console
< 2025/11/09 20:18:24.000428478  length=10 from=0 to=9
 fe ed 01 02 21 00 f2 ba f7 fa                    ....!.....
--
> 2025/11/09 20:18:24.000431576  length=10 from=0 to=9
 fe ed 01 02 21 00 f2 ba f7 fa                    ....!.....
--
```

