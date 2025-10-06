
#include "glad/glad.h"

#include "window.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

const int WINDOW_WIDTH = 1000;
const float ASPECT_RATIO = 16.0 / 9.0;

void framebuffer_size_callback(WindowPtr *window, int width, int height);
void glfw_error_callback(int error, const char *description) {
  std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

WindowPtr *init_window(string title, int width, int height, bool floating) {

  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW!" << std::endl;
    return nullptr;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_FLOATING, floating ? GLFW_TRUE : GLFW_FALSE);

  WindowPtr *window = glfwCreateWindow(width, height, "Goose", NULL, NULL);

  if (window == NULL) {
    const char *description;
    int code = glfwGetError(&description);
    cout << "Failed to create window: " << description
         << " (Error Code: " << code << ")" << endl;
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);
  glfwSetErrorCallback(glfw_error_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to load GLAD" << endl;
    glfwTerminate();
    return NULL;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  return window;
}

void framebuffer_size_callback(WindowPtr *window, int width, int height) {
  glViewport(0, 0, width, height);
}
