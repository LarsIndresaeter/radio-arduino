from conans import ConanFile, CMake, tools
from conans.tools import os_info, SystemPackageTool
import os

def gitSemVerAddGitSha():
    git = tools.Git() 

    versionString = str(git.run("describe --tags --abbrev=0"))

    try:
        gitsha = str(git.run("log --pretty=format:'%h' -n 1"))
        commits_not_in_main = int(git.run(f"rev-list --count HEAD --not main"))

        versionString += f".{commits_not_in_main}"
        versionString += f"+{gitsha}"

        filesNotCommited = str(git.run("status --short"))
        if len(filesNotCommited) > 0:
            versionString += "-dirty"
    except:
        pass

    return versionString

class AvrUartConan(ConanFile):
    name = "raduino-avr"
    version=gitSemVerAddGitSha()
    license = "gpl"
    author = "Lars Indresaeter"
    url = "https://github.com/LarsIndresaeter/snippets.git"
    description = "avr snippet using conan and cmake"
    topics = ("conan", "avr")
    settings = "build_type"
    options = {}
    generators = "cmake"
    keep_imports = True
    exports_sources = [ 
            "CMakeLists.txt",
            "libs/*",
            "third_party_libs/*",
            "cmake/*",
            "include/*",
            "src/*",
            "unittest/*",
            ]

    def requirements(self):
        self.requires("raduino-api/0.0.1@raduino/test")

    def imports(self):
        self.copy("*.hpp", dst="libs", src="libs", root_package="raduino-api")
        self.copy("*.hxx", dst="libs", src="libs", root_package="raduino-api")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["ARDUINO_VERSION"] = self.version
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.hex", dst="bin", src="bin", keep_path=False)

    def package_info(self):
        self.cpp_info.includedirs = ["bin"]

    def system_requirements(self):
        packages = None
        if os_info.linux_distro == "ubuntu":
            installer = SystemPackageTool()
            installer.install("cmake")
            installer.install("ccache")
            installer.install("gcc-avr")
            installer.install("avr-libc")
            installer.install("avrdude")

