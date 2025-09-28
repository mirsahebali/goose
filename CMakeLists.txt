cmake_minimum_required(VERSION 3.15)
project(goose CXX)


# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
include_directories(include)
include_directories(deps)

add_executable(${PROJECT_NAME}
                src/window.cc
                src/shaders.cc
                src/textures.cc
                src/image.cc
                app.cc)

add_executable(${PROJECT_NAME}-tests
                src/window.cc
                src/shaders.cc
                src/textures.cc
                src/image.cc
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
