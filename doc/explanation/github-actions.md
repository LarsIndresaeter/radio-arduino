# Github actions

## dockerapp images

Docker apps are used for programming rf-nano and interacting with is.
Building of this image is triggered by tags in the format `major.minor.patch`.

[List of dockerapp images](https://github.com/LarsIndresaeter/radio-arduino/pkgs/container/radio-arduino)

To start this image run the command

```console
docker run --rm --device=/dev/ttyUSB0 -it ghcr.io/larsindresaeter/radio-arduino:1.7.0 bash
```

If you are not familiar with [podman](https://podman.io/) you might want to
evaluate this for standalone nodes.

### devbox

Devbox is just another name for a container used for building software.
Building of this images is triggered by changes to the file `tools/docker/dev/Dockerfile`.

[List of devbox images](https://github.com/LarsIndresaeter/radio-arduino/pkgs/container/radio-arduino-devbox)

```console
docker run -v .:/home/raduino/ -it ghcr.io/larsindresaeter/radio-arduino-devbox:1.0.0 bash
```

in the container

```console
source tools/setup.sh
raduino help
```

This will display the help menu

```console
raduino build menu
    format <code>
    lint <doc>
    generate <commands>
          run code generator for commands
    dockerapp <build|run>
          build or run a container with raduino apps
    devbox start up a container where you can build and run this software
    tool <personalize|gateway|node|test|mqtt|power>
    setup <arduino|interface|linux|all>
          set up build area using conan and cmake
    clean <arduino|interface|linux|all>
          clean build area
    build <arduino|interface|linux|all>
          build target in area created by setup
    all
          run commands: clean, setup and build
    release <bump|push|detect|latest>
    flash <rf-nano|nano-pro>
          flash arduino
    mqtt
          mqtt commands
    conan
          create conan packages
```

Typical commands are

* `raduino all` for clean build
* `raduino build linux` for incremental build of the Linux tools
* `raduino flash rf-nano /dev/ttyUSB0` for flashing a rf-nano board

You can read more about the raduino toolkit [here](../explanation/raduino-toolkit.md).

