cmake_minimum_required(VERSION 3.15)
project(goose CXX)

include(FetchContent)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
    GIT_SHALLOW TRUE
)

set(FETCHCONTENT_QUIET FALSE)
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/deps")
FetchContent_MakeAvailable(imgui)

file(DOWNLOAD
      "https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h"
      deps/stb_image.h
)

include_directories(include)
include_directories(include/core)
include_directories(deps)
include_directories(${imgui_SOURCE_DIR})
include_directories(${imgui_SOURCE_DIR}/backends)


# Add ImGui source files to the project
set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/backends/imgui_impl_opengl3.cpp
)

set(APP_CORE
            src/core/window.cc
            src/core/shaders.cc
            src/core/textures.cc
            src/core/image.cc
            src/core/utils.cc
            src/core/draw.cc
            src/core/camera.cc
            src/core/gsmath.cc)

add_executable(${PROJECT_NAME}
                ${APP_CORE}
                ${IMGUI_SOURCES}
                app.cc)

add_executable(${PROJECT_NAME}-tests
                ${APP_CORE}
                ${IMGUI_SOURCES}
                tests.cc)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glad::glad
    PUBLIC
    glfw
    glm::glm
)

target_link_libraries(${PROJECT_NAME}-tests
    PRIVATE glad::glad
    PUBLIC
    glfw
    glm::glm
)
