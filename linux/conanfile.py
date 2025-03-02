from conans import ConanFile, CMake, tools
from conans.tools import os_info, SystemPackageTool

class AvrUartConan(ConanFile):
    name = "uart-tool"
    version = "0.0.1"
    license = "gpl"
    author = "Lars Indresaeter"
    url = "https://github.com/LarsIndresaeter/snippets.git"
    description = "uart tool written in c++"
    topics = ("conan", "uart")
    # settings = "build_type"  
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False], "mqtt": [True, False]}
    default_options = {"shared": False, "fPIC": True, "mqtt": False}
    generators = "cmake", "cmake_find_package"
    keep_imports=True
    exports_sources = [ 
            "CMakeLists.txt",
            "libs/*",
            "apps/*",
            "src/*",
            ]

    def requirements(self):
        if self.options.mqtt == True:
            self.requires("paho-mqtt-cpp/1.2.0@", private=True)
        else:
            self.requires("openssl/3.1.1",private=True)
        self.requires("uart-api/0.0.1@lars/test",private=True)
        self.requires("uart-avr/[>0.0.2, include_prerelease=True]@lars/test",private=True)

    def imports(self):
        self.copy("*.a", dst="", src="")
        self.copy("*.hpp", dst="libs", src="libs", root_package="uart-api")
        self.copy("*", dst="bin", src="bin")

    def build(self):
        cmake = CMake(self)
        if self.options.mqtt == True:
            cmake.definitions["mqtt"] = 1
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("uart-tool", dst="bin", src="apps/generic/bin")
        self.copy("arduino.hex", dst="bin", src="bin")

    def system_requirements(self):
        packages = None
        if os_info.linux_distro == "ubuntu":
            installer = SystemPackageTool()
            installer.install("cmake")
            installer.install("ccache")
