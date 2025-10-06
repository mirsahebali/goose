#pragma once

#include <iostream>
#include <string>

#include <GLFW/glfw3.h>

using std::cout;
using std::endl;
using std::string;

typedef struct WindowConfig {
  std::string name;
  float aspect_ratio;
  int width;
  int height;
  bool floating;
} WindowConfig;

extern WindowConfig window_cfg;

typedef GLFWwindow WindowPtr;

WindowPtr *init_window(string title, int width, int height, bool floating);
