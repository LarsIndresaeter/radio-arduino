# develop on mac

install docker. Docker Desktop or OrbStack.

## create docker image

```console
./tools/devbox.sh build
```

## start devbox

```console
./tools/devbox.sh run
```

## build

```console
raduino all
```

## known issues

List of known issues

- paho-mqtt-c does not compile for docker on mac with m2
- access to trusted ch340 driver seems to be lacking

