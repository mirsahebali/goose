
#include "app.h"
#include "colors.h"
#include "draw.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <functional>

#include "cumin.h"

#define VERTEX_PATH "shaders/primitive_vertex.glsl"
#define FRAGMENT_PATH "shaders/primitive_fragment.glsl"

#define NEW_PRIMITIVE_SHADER new Shader(VERTEX_PATH, FRAGMENT_PATH)
const int WINDOW_WIDTH = 1000;
const float ASPECT_RATIO = 16 / 9.0;
const int WINDOW_HEIGHT = WINDOW_WIDTH * (1 / ASPECT_RATIO);

WindowConfig window_cfg = {
    .name = "Cumin App",
    .aspect_ratio = ASPECT_RATIO,
    .width = WINDOW_WIDTH,
    .height = WINDOW_HEIGHT,
    .floating = true,
};

int main(int argc, char *argv[]) {

  CuminApp app(window_cfg);
  app.on_startup();
  app.run();

  return 0;
}

AppState::AppState()
    : circle_renderer(NEW_PRIMITIVE_SHADER, 100, window_cfg.width,
                      window_cfg.height),
      triangle_renderer(NEW_PRIMITIVE_SHADER, window_cfg.width,
                        window_cfg.height),
      quad_renderer(NEW_PRIMITIVE_SHADER, window_cfg.width, window_cfg.height),
      line_renderer(NEW_PRIMITIVE_SHADER, window_cfg.width, window_cfg.height),
      font_renderer(
          new Shader("shaders/font_vertex.glsl", "shaders/font_fragment.glsl"),
          "assets/font/ToThePoint.ttf", window_cfg.width, window_cfg.height) {
  background_color = WHITE;
  animation = 0;
}

CuminApp::CuminApp(WindowConfig _window_config)
    : App(_window_config), state(AppState()) {}

void CuminApp::clear_screen() {
  Color bg_color = state.background_color;
  glClearColor(bg_color.r, bg_color.g, bg_color.b, bg_color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CuminApp::render() {
  clear_screen();

  // Color color)
  state.line_renderer.draw2d(-100, 0, 1000, 100, 0, BLUE);
  state.font_renderer.draw("Hello, OpenGL", 0, 0, 0.5, BLACK);
  state.font_renderer.draw("Hello, OpenGL 2", WINDOW_WIDTH / 2, 0, 0.5, BLACK);
  draw_grid(10);
}

void CuminApp::draw_grid(int pixel_size) {
  // // draw rows
  // for (int i = -window_cfg.width / 2; i < window_cfg.width / 2; i++) {
  // }
}

void CuminApp::process_event(AppEvent e) {
  if (glfwGetKey(global.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(global.window, true);
  }
}

void CuminApp::on_update(float ns) {}
