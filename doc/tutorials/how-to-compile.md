# how to compile

For local development you need to install conan. 

## build using conan

```console
source tools/setup.sh local
raduino all
```

## compile the code in a docker container

Docker is useful for continouous integration or if you don't want to install conan on your system. 

### install docker

1. install docker
2. create image

```console
./tools/docker/build_in_docker.sh dockerbuild
```

### build using docker

```console
source tools/setup.sh docker
raduino all
```

### build and output folder

a successful build should result in two folders beeing created:

| folder | description                                                  |
| ------ | ------------------------------------------------------------ |
| build/ | local build area unless you are creating conan packages      |
| bin/   | compiled binary files unless you are creating conan packages |

Note that if you are creating conan packages then the files will be in your local conan cache. This tutorial will not teach you the details of how conan works. docs.conan.io is great for learning how conan works.

