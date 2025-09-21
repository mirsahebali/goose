cmake_minimum_required(VERSION 3.15)
project(goose CXX)


## INFO: for C++ project
# project(raylib_template VERSION 1.0)
#
# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(include)
include_directories(deps)

add_executable(${PROJECT_NAME} src/window.cc src/shaders.cc app.cc)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glad::glad
    PUBLIC glfw
)

