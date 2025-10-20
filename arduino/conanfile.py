from conans import ConanFile, CMake, tools
from conans.tools import os_info, SystemPackageTool
import os

def gitSemVer():
    git = tools.Git() 

    # The environment variable BUILD_NUMBER will be present Jenkins created this package
    build_number = str(os.getenv("BUILD_NUMBER", "dirty"))

    try:
        current_tag = git.run("describe --tags --abbrev=0 2> /dev/null")
        ommits_ahead_of_tag = int(git.run(f"rev-list --count {current_tag}..HEAD"))
        gitsha = str(git.run("log --pretty=format:'%h' -n 1"))
        versionString += f"+sha.{gitsha}"
    except:
        return "0.0.0"

    versionString = f"{current_tag}-{build_number}.{commits_ahead_of_tag}+sha.{gitsha}"

    return versionString

def gitSemVerAddGitSha(versionString):
    git = tools.Git() 

    # The environment variable BUILD_NUMBER will be present Jenkins created this package
    build_number = str(os.getenv("BUILD_NUMBER", "dirty"))

    try:
        gitsha = str(git.run("log --pretty=format:'%h' -n 1"))
        commits_not_in_main = int(git.run(f"rev-list --count HEAD --not main"))

        versionString += f".{commits_not_in_main}"
        versionString += f"+{gitsha}-{build_number}"
    except:
        pass

    return versionString

class AvrUartConan(ConanFile):
    name = "raduino-avr"
    version=gitSemVerAddGitSha('0.1.0')
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
        self.requires("raduino-api/0.0.1@lars/test")

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

