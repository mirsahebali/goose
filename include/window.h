#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

typedef GLFWwindow Window;

Window *init_window(string title, int width, int height, bool floating);
