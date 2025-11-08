# cmake and conan

## calling cmake from conan

the file `arduino/conanfile.py` use the file `arduino/CMakeLists.txt` for setting up the build using the toolchain defined in the profile `tools/conan/profile/gcc-avr`.

```python
    def build(self):
        cmake = CMake(self)
        cmake.definitions["ARDUINO_VERSION"] = self.version
        cmake.configure()
        cmake.build()
```

This topic is rather large and will not be covered in detail in this documentation. Please refer to docs.conan.io for more information.

