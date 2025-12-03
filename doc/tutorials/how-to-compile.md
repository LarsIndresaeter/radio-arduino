# how to compile

For local development you need to install conan.

## setup

### build in docker

start a container and use it to build

```console
./tools/devbox.sh run
```

### build using conan

```console
source tools/setup.sh
```

## build

clean build

```console
raduino all
```

more commands in the raduino toolkit

```console
raduino help
```

### build and output folder

a successful build should result in two folders beeing created:

| folder | description                                                  |
| ------ | ------------------------------------------------------------ |
| build/ | local build area unless you are creating conan packages      |
| bin/   | compiled binary files unless you are creating conan packages |

Note that if you are creating conan packages then the files will be in your
local conan cache. This tutorial will not teach you the details of how conan
works. docs.conan.io is great for learning how conan works.

### build time

clean build on i5-1335, command `raduino all`.

- first time: 2m 10s
- clean build: 13s
- incremental build: 2s

