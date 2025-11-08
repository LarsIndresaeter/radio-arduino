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

class UartApiConan(ConanFile):
    name = "raduino-api"
    version=gitSemVerAddGitSha()
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
        self.copy("*.hxx", dst="libs/", src="libs/")

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
