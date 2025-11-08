from conans import ConanFile, CMake, tools
from conans.tools import os_info, SystemPackageTool

def gitSemVerAddGitSha():
    git = tools.Git() 

    versionString = ""

    try:
        versionString = str(git.run("describe --tags --abbrev=0"))

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
    name = "raduino-gateway"
    version=gitSemVerAddGitSha()
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
            self.requires("nlohmann_json/3.11.3", private=True)
        else:
            self.requires("openssl/3.1.1",private=True)
        self.requires("raduino-api/[>0.0.2, include_prerelease=True]@raduino/test",private=True)
        self.requires("raduino-avr/[>0.0.2, include_prerelease=True]@raduino/test",private=True)

    def imports(self):
        self.copy("*.a", dst="", src="")
        self.copy("*.hpp", dst="libs", src="libs", root_package="raduino-api")
        self.copy("*.hxx", dst="libs", src="libs", root_package="raduino-api")
        self.copy("*", dst="bin", src="bin")

    def build(self):
        cmake = CMake(self)
        if self.options.mqtt == True:
            cmake.definitions["mqtt"] = 1
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("raduino-gateway", dst="bin", src="apps/generic/bin")

    def system_requirements(self):
        packages = None
        if os_info.linux_distro == "ubuntu":
            installer = SystemPackageTool()
            installer.install("cmake")
            installer.install("ccache")
