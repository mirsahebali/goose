from conan import ConanFile
from conan.tools.cmake import cmake_layout


class GooseRecipie(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("glad/0.1.36")
        self.requires("glfw/3.4")
        self.requires("glm/1.0.1")
        self.requires("imgui/1.92.2b")
        self.requires("freetype/2.13.3")
