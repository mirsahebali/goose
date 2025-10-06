#pragma once

#include "camera.h"
#include "window.h"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "imgui.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

using vec3 = glm::vec3;
using mat4 = glm::mat4;

typedef enum class KeyEvent : u8 {
  KeyPress,
  KeyRelease,
  KeyDown,
  KeyHold,
  KeyRepeat
} KeyEvent;

typedef enum class MouseEvent : u8 {
  LeftButton,
  MiddleButton,
  ScrollButton,
  MouseScroll,
  MouseMovement
} MouseEvent;

typedef enum class Event : u8 {
  KeyEvent,
  MouseEvent,
  GamepadEvent,
} Event;

typedef struct {
  Camera camera;
  float delta_time;
  float last_frame_time;
  bool first_mouse_enter;
  WindowPtr *window;
  std::string app_name;
} Globals;

template <typename Event> class App {
public:
  App(WindowConfig _window_config);

  virtual ~App();

  Globals global;
  WindowConfig window;
  Event current_event;

  void on_startup();
  void begin_rendering();
  void end_rendering();
  void run();

  virtual void render();
  virtual void on_update(float ns);
  virtual void process_event(Event);
  virtual void on_shutdown();
};

template <typename E>
App<E>::App(WindowConfig _window_cfg) : window(_window_cfg) {

  WindowPtr *win =
      init_window(window.name, window.width, window.height, window.floating);

  if (win == nullptr) {
    std::cout << "Cannot initialize window" << std::endl;
    return;
  }

  global.window = win;
  glfwMakeContextCurrent(global.window);
  glEnable(GL_DEPTH_TEST);
}

// overridable functions
template <typename E> void App<E>::on_update(float ns) {}

template <typename E> void App<E>::process_event(E e) {
  if (glfwGetKey(global.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(global.window, true);
  }
}

template <typename E> void App<E>::on_startup() {}

template <typename E> void App<E>::begin_rendering() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}
template <typename E> void App<E>::end_rendering() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(global.window);
  glfwPollEvents();
}

template <typename E> void App<E>::on_shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(global.window);
  glfwTerminate();
}

template <typename E> void App<E>::render() {
  // rendering
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

template <typename E> void App<E>::run() {
  on_startup();
  while (!glfwWindowShouldClose(global.window)) {
    begin_rendering();

    float current_time = glfwGetTime();
    global.delta_time = current_time - global.last_frame_time;
    global.last_frame_time = current_time;

    process_event(current_event);
    on_update(global.delta_time);

    render();

    end_rendering();
  }
}

template <typename E> App<E>::~App() { on_shutdown(); }
