# Install tools

## build docker image used to build

build the docker image for building software

```console
./tools/devbox.sh build
```

## install conan

To make it easy to get started the build system (conan) will install missing
tools. It should idealy be enough to install conan on your developer machine or docker.
Conanfiles are currently using version 1.66. An example of how this installed
can be seen in the Dockerfile in `tools/docker/Dockerfile`

```console
pip3 install --break-system-packages conan==1.66
```

## Other tools

Docker build calls conan and conan will install these tools if they are missing.

- cmake
- ccache
- gcc-avr
- avr-libc
- avrdude
- gtest

