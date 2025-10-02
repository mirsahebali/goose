// goose includes
#include "app.h"
#include "colors.h"
#include "draw.h"
#include "shaders.h"
#include "textures.h"
#include "window.h"

// external includes
#include <GLFW/glfw3.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>

#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

float mix_value = 0.0;

mat4 view = mat4(1.0f);
float radius = 10;

vec3 camera_pos = vec3(0.0f, 0.0f, 2.0f);
vec3 world_up = vec3(0.f, 1.f, 0.f);
vec3 camera_front = vec3(0.0f, 0.0f, -1.0f);
vec3 camera_target = vec3(0.0f, 0.0f, 0.0f);

float delta_time = 0.0f;
float last_frame_time = 0.0f;

float last_mouse_x = WINDOW_WIDTH / 2.0;
float last_mouse_y = WINDOW_HEIGHT / 2.0;
bool first_mouse_enter = true;

int main(int argc, char *argv[]) {

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  Window *window = init_window("Goose", WINDOW_WIDTH, WINDOW_HEIGHT, true);
  if (window == NULL) {
    cout << "Failed to open window" << endl;
    return -1;
  }

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  // glfwSetCursorPosCallback(window, mouse_callback);
  // glfwSetScrollCallback(window, scroll_callback);

  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);

  Shader *line_shader = new Shader("./shaders/primitive_vertex.glsl",
                                   "./shaders/primitive_fragment.glsl");

  Shader *circle_shader = new Shader("./shaders/primitive_vertex.glsl",
                                     "./shaders/primitive_fragment.glsl");

  Shader *triangle_shader = new Shader("./shaders/primitive_vertex.glsl",
                                       "./shaders/primitive_fragment.glsl");

  Shader *quad_shader = new Shader("./shaders/primitive_vertex.glsl",
                                   "./shaders/primitive_fragment.glsl");
  mat4 model = mat4(1.0f);

  mat4 projection = glm::ortho(
      -(float(WINDOW_WIDTH) / 2), float(WINDOW_WIDTH) / 2,
      -(float(WINDOW_HEIGHT) / 2), float(WINDOW_HEIGHT) / 2, 0.1f, 100.0f);

  // line_shader->set_mat4("MVPprojections", model * view * projection);

  int num_segments_for_circle = 100;
  LineRenderer line_renderer(line_shader);
  CircleRenderer circle_renderer(circle_shader, num_segments_for_circle);
  TriangleRenderer triangle_renderer(triangle_shader);
  QuadRenderer quad_renderer(quad_shader);

  char buf[100];
  float f = 0.0;

  while (!glfwWindowShouldClose(window)) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    float current_time = glfwGetTime();
    delta_time = current_time - last_frame_time;
    last_frame_time = current_time;

    // input
    process_input(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    line_renderer.draw2d(1.0f, 1.0f, -1.0f, -1.0f, 1.0, CYAN);
    circle_renderer.draw2d(0.2f, -0.2f, .3f, GREEN, false);
    circle_renderer.draw2d(0.0f, 0.0f, .3f, BLUE, true);
    triangle_renderer.draw2d(-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f,
                             0.0, RED, true);

    quad_renderer.draw2d(0.0f, 0.0f, 0.8f, 0.8f, WHITE, true);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  delete line_shader;
  delete circle_shader;

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  return 0;
}

void process_input(Window *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void mouse_callback(Window *window, double x_pos_in, double y_pos_in) {
  float x_pos = float(x_pos_in);
  float y_pos = float(y_pos_in);

  if (first_mouse_enter) {
    last_mouse_x = x_pos;
    last_mouse_y = y_pos;
    first_mouse_enter = false;
  }

  float x_offset = x_pos - last_mouse_x;
  float y_offset = last_mouse_y - y_pos;
  last_mouse_x = x_pos;
  last_mouse_y = y_pos;
}

void scroll_callback(Window *window, double x_offset, double y_offset) {}
