from conans import ConanFile, CMake, tools
from conans.tools import os_info, SystemPackageTool

class AvrUartConan(ConanFile):
    name = "uart-api-test"
    version = "0.0.1"
    license = "gpl"
    author = "Lars Indresaeter"
    url = "https://github.com/LarsIndresaeter/snippets.git"
    description = "avr snippet using conan and cmake"
    topics = ("conan", "avr")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = [ 
            "CMakeLists.txt",
            "*.cpp",
            ]
    test_type = "uart-api/0.0.1@lars/test"

    def requirements(self):
        self.requires("gtest/1.8.1")
        self.requires("uart-api/0.0.1@lars/test",private=True)

    def build_requirements(self):
        self.build_requires("gtest/1.8.1", force_host_context=True)

    def imports(self):
        self.copy("*.hpp", dst="libs", src="libs", root_package="uart-api")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*", dst="bin", keep_path=False)

    def test(self):
        self.run("bin/unittest")

    def system_requirements(self):
        packages = None
        if os_info.linux_distro == "ubuntu":
            installer = SystemPackageTool()
            installer.install("cmake")

