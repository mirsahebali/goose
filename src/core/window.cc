#include "window.h"
#include <GLFW/glfw3.h>

void framebuffer_size_callback(Window *window, int width, int height);

Window *init_window(string title, int width, int height, bool floating) {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_FLOATING, floating ? GLFW_TRUE : GLFW_FALSE);

  Window *window = glfwCreateWindow(width, height, "Goose", NULL, NULL);

  if (window == NULL) {

    cout << "Failed to init window " << endl;
    glfwTerminate();

    return NULL;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

    cout << "Failed to load GLAD" << endl;
    glfwTerminate();
    return NULL;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return window;
}

void framebuffer_size_callback(Window *window, int width, int height) {
  glViewport(0, 0, width, height);
}
