cmake_minimum_required(VERSION 3.15)
project(goose VERSION 0.1 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Set base dir for dependencies
set(FETCHCONTENT_QUIET FALSE)
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/deps")

# === Fetch ImGui ===
FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
    GIT_TAG docking
    GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(imgui)

# stb_image.h
file(DOWNLOAD
    "https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h"
    ${CMAKE_SOURCE_DIR}/deps/stb_image.h
)

file(DOWNLOAD
    "https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_truetype.h"
    ${CMAKE_SOURCE_DIR}/deps/stb_truetype.h
)

# Core source files
set(APP_CORE
    src/core/window.cc
    src/core/shaders.cc
    src/core/textures.cc
    src/core/image.cc
    src/core/utils.cc
    src/core/draw.cc
    src/core/camera.cc
    src/core/app.cc
    src/core/gsmath.cc
    src/core/truetype.cc
)

# ImGui source files
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# Find other packages
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(freetype REQUIRED)

# === Core Library ===
add_library(goose-core
    ${APP_CORE}
    ${IMGUI_SOURCES}
)

target_include_directories(goose-core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/core
        ${CMAKE_SOURCE_DIR}/deps
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(goose-core
    PRIVATE glad::glad
    PUBLIC glfw glm::glm
    Freetype::Freetype
)

# === Executables ===

add_executable(goose-tests tests.cc)
target_link_libraries(goose-tests PRIVATE goose-core)

# add_executable(goose-demo example_app.cc)
# target_link_libraries(goose-demo PRIVATE goose-core)

include_directories(src/app)
add_executable(cumin src/app/cumin.cc)
target_link_libraries(cumin PRIVATE goose-core)
target_include_directories(cumin PRIVATE ${CMAKE_SOURCE_DIR}/src/app)
