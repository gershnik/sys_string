from conans import ConanFile, CMake, tools


class SysStringConan(ConanFile):
    name = "sys_string"
    version = "0.1"
    license = "BSD 3-Clause License"
    author = "Eugene Gershnik gershnik@hotmail.com"
    url = "https://github.com/gershnik/sys_string"
    description = "A pointer-sized, immutable, Unicode-first C++ string class that is optimized for interoperability with platform-native string types."
    topics = ("string class", "unicode")
    settings = "cppstd", "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def source(self):
        self.run("git clone https://github.com/gershnik/sys_string.git")
        # This small hack might be useful to guarantee proper /MT /MD linkage
        # in MSVC if the packaged project doesn't have variables to set it
        # properly
        tools.replace_in_file("sys_string/lib/CMakeLists.txt", "project(sys_string)",
                              '''project(sys_string)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()''')

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="sys_string/lib")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="inc", src="sys_string/lib/inc")
        self.copy("*sys_string.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.includedirs = ["inc"]
        self.cpp_info.libs = ["sys_string"]

        if self.settings.os in ['Macos', 'iOS', 'tvOS', 'watchOS']:
            self.cpp_info.exelinkflags.append("-framework CoreFoundation")
            self.cpp_info.sharedlinkflags = self.cpp_info.exelinkflags
        elif self.settings.os.startswith('Windows'):
            self.cpp_info.libs.append("runtimeobject")

