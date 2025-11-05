# Install tools

To make it easy to get started the build system (conan) will install missing tools. It should idealy be enough to install conan on your developer machine or docker.

## install conan

Conanfiles are currently using version 1.57. An example of how this installed can be seen in the Dockerfile in `tools/docker/Dockerfile`

```console
RUN pip3 install conan==1.57
```

## install docker

Docker can be used for building. The intended use case is for a build server in Continouous Integration (CI), but it can also be useful if you are struggeling to install conan.

```console
sudo groupadd docker
sudo usermod -aG docker $USER
```

## Other tools

Docker build calls conan and conan will install missing tools like:

* cmake
* ccache
* gcc-avr
* avr-libc
* avrdude
* gtest
  
