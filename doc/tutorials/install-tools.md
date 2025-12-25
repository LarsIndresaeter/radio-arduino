# Install tools

## pull radio-arduino-devbox from ghcr.io

The easiest way to compile is to use `devbox` build by
[github actions](../explanation/github-actions.md) and downloaded from ghcr.io.

```console
docker run -v /path/to/radio-arduino/:home/raduino/ -it ghcr.io/larsindresaeter/radio-arduino-devbox:1.0.0 bash
```

There are however many use cases where you want to build `devbox` yourself or
install the tools on your developer machine.

## build radio-arduino-build locally

build the devbox docker image for compiling software.

```console
./tools/devbox.sh build
```

use devbox

```console
./tools/devbox.sh run
```

> Note that the image is named `radio-arduino-devbox` on ghcr.io and
> `radio-arduino-build` when build locally.

## install local build tools

Your third option is to install conan on your developer machine.
The devbox docker image is using conan so the result will be the same with or
without docker.
Conan will in turn install missing tools.

```console
pip3 install --break-system-packages conan==1.66
```

## Other tools

Conan will install these tools if they are missing.

- cmake
- ccache
- gcc-avr
- avr-libc
- avrdude
- gtest

