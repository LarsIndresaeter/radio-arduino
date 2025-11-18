# conan

## editable mode

### setup

the command `raduino setup <target>` will setup the package for editable mode

```console
setup
    interface : clean interface build
      arduino : clean arduino build
        linux : clean linux build
          all : clean all builds
         help : print this menu
```

If you wish to run the commands yourself then you need to reference the layout
files located in `tools/conan/layouts/`

### build editable mode

The command `raduino build <target>` will build the package configured for
editable mode

```console
clean
    interface : clean interface build
      arduino : clean arduino build
        linux : clean linux build
          all : clean all builds
         help : print this menu
```

## build packages using raduino command

The command `raduino conan` can be used to build single packages or all
packages. For example `raduino conan interface` will build the `raduino-api`
package.

```console
conan_package
    interface : create interface package
      arduino : create arduino package
        linux : create linux package
          all : create all packages
         help : print this menu
```

### setup and build in one step

the command `raduino all` will run the commands `raduino setup all` and
`raduino build all`. This is the command you will use the most often during development.

## build using conan command

### create raduino-api package

```console
cd interface
conan create . lars/test
```

or simply use the command `raduino conan interface`.

this should result in a package in your conan commandTest.

```console
conan search raduino-api
raduino-api/0.4.0.4+206cf7e-dirty@raduino/test
```

### create raduino-avr package

```console
cd arduino
conan create . lars/test -pr ../tools/conan/profiles/gcc-avr
```

or simply use the command `raduino conan arduino`.

this should result in a package in your conan commandTest.commandEepromReadHigh

```console
conan search raduino-avr
raduino-avr/0.4.0.4+206cf7e-dirty@lars/test
```

### create raduino-gateway package

```console
cd linux
conan create . lars/test
```

or simply use the command `raduino conan linux`.

this should result in a package in your conan commandTest.commandEepromReadHigh

```console
conan search raduino-gateway
raduino-gateway/0.4.0.4+206cf7e-dirty@lars/test
```

## calling cmake from conan

the file `arduino/conanfile.py` use the file `arduino/CMakeLists.txt` for
setting up the build using the toolchain defined in the profile `tools/conan/profile/gcc-avr`.

```python
    def build(self):
        cmake = CMake(self)
        cmake.definitions["ARDUINO_VERSION"] = self.version
        cmake.configure()
        cmake.build()
```

This topic is rather large and will not be covered in detail in this
documentation. Please refer to [docs.conan.io](https://docs.conan.io/en/1.57/index.html)
for more information.

