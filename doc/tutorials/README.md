# tutorials

Lessons that take the reader by the hand through a series of steps to complete a project.

## getting started

You can build or run the apps nativly or inside a docker container.

### using docker

1. [docker app](./docker-app.md) is the fastest and easiest way to try this
sofware. Almost no installation required.

#### devbox

You can use docker for compling

```console
cd radio-arduino/
./tools/devbox.sh build
./tools/devbox.sh run
```

you will no enter a docker container with all the tools needed. This has been
tested to work on an x86 PC and [Raspberry Pi](../how-to-guides/develop-on-raspberry-pi.md).
There are currently [some issues on mac](../how-to-guides/develop-on-mac.md).

### native build

1. [install tools](./install-tools.md) needed to compile and flash the arduino.
2. [how to compile](./how-to-compile.md)) software using the toolkit in this repository.
3. [flash arduino](./flash-arduino.md) boards using command line tools.

### using the linux apps

1. [how to configure](./configure-device.md) a newly programmed board as gateway
   or node.
2. [list of apps](../reference-guides/apps/README.md) used to interact with the arduinos.

