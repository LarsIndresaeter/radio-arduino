# how to compile

For local development you need to install Conan.

## setup

### build in docker

Start a container and use it to build

```console
./tools/devbox.sh run
```

### build using Conan

```console
source tools/setup.sh
```

## build

Clean build

```console
raduino all
```

More commands in the raduino toolkit

```console
raduino help
```

### build and output folder

A successful build should result in two folders being created:

| folder | description                                                  |
| ------ | ------------------------------------------------------------ |
| build/ | local build area unless you are creating Conan packages      |
| bin/   | compiled binary files unless you are creating Conan packages |

Note that if you are creating Conan packages then the files will be in your
local Conan cache. This tutorial will not teach you the details of how Conan
works. docs.Conan.io is great for learning how Conan works.

### build time

Clean build on i5-1335, command `raduino all`.

- first time: 2m 10s
- clean build: 13s
- incremental build: 2s

