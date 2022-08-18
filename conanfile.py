from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class KieToolboxConan(ConanFile):
    name = "kie_toolbox"
    version = "0.1.1"

    # Optional metadata
    license = "MIT"
    author = "Kie <qiongxiaozi158@sina.com>"
    url = "https://github.com/Kidsunbo/KieContext"
    description = "Kie's toolbox for CPP"
    topics = ("Kie", "toolbox", "CPP")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "enable_test": [True, False],
        "with_serde": [True, False],
        "with_serde_json": [True, False],
        "with_context": [True, False],
        "with_container": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "enable_test": True,
        "with_serde": False,
        "with_serde_json": False,
        "with_context": False,
        "with_container": False
    }

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "test/*", "include/*", "LICENSE"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("boost/1.79.0")
        if self.options.with_serde_json:
            self.requires("nlohmann_json/3.11.2")

    def build_requirements(self):
        if self.options.enable_test:
            self.test_requires("gtest/cci.20210126")

    def _configure(self, tc):
        tc.variables["ENABLE_TEST"] = self.options.enable_test
        tc.variables["WITH_CONTEXT"] = self.options.with_context
        tc.variables["WITH_SERDE"] = self.options.with_serde
        tc.variables["WITH_CONTAINER"] = self.options.with_container
        if self.options.with_serde_json:
            tc.variables["WITH_SERDE"] = True
            tc.variables["WITH_SERDE_JSON"] = True

    def generate(self):
        tc = CMakeToolchain(self)
        self._configure(tc)
        tc.generate()

        tc = CMakeDeps(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if self.options.enable_test:
            cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        if True:
            # For now, this is a header only library
            self.info.clear()
        else:
            self.cpp_info.libs = ["kie_toolbox"]
