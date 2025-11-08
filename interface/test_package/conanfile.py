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
    name = "raduino-api-test"
    version=gitSemVerAddGitSha()
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
    test_type = "raduino-api/0.1.0@raduino/test"

    def requirements(self):
        self.requires("gtest/1.8.1")
        self.requires("raduino-api/[>0.0.2, include_prerelease=True]@raduino/test",private=True)

    def build_requirements(self):
        self.build_requires("gtest/1.8.1", force_host_context=True)

    def imports(self):
        self.copy("*.hpp", dst="libs", src="libs", root_package="raduino-api")
        self.copy("*.hxx", dst="libs", src="libs", root_package="raduino-api")

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

