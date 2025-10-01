#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

typedef GLFWwindow Window;

// const float ASPECT_RATIO = 16.0 / 9.0;
const float ASPECT_RATIO = 1.0f;

const uint WINDOW_WIDTH = 1000;
const uint WINDOW_HEIGHT = WINDOW_WIDTH * (1 / ASPECT_RATIO);

Window *init_window(string title, int width, int height, bool floating);
