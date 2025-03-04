from conans import ConanFile, CMake, tools
from conans.tools import os_info, SystemPackageTool

class UartApiConan(ConanFile):
    name = "uart-api"
    version = "0.0.1"
    license = "gpl"
    author = "Lars Indresaeter"
    url = "https://github.com/LarsIndresaeter/snippets.git"
    description = "avr snippet using conan and cmake"
    topics = ("conan", "avr")
    settings = "build_type"
    generators = "cmake"
    exports_sources = [ 
            "CMakeLists.txt",
            "libs/*",
            ]

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.hpp", dst="libs/", src="libs/")

    def package_info(self):
        self.cpp_info.includedirs = ["libs/protocol/include"]
        self.cpp_info.includedirs = ["libs/commands/include"]

    def system_requirements(self):
        packages = None
        if os_info.linux_distro == "ubuntu":
            installer = SystemPackageTool()
            installer.install("cmake")
            installer.install("ccache")
            installer.install("ninja-build")
