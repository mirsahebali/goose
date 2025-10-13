
#include "IconsCodicons.h"
#include "IconsFontAwesome5.h"
#include "IconsLucide.h"

#include "app.h"
#include "colors.h"
#include "draw.h"

#include "imgui.h"
#include "textures.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "cumin.h"

#define VERTEX_PATH "shaders/primitive_vertex.glsl"
#define FRAGMENT_PATH "shaders/primitive_fragment.glsl"

#define NEW_PRIMITIVE_SHADER new Shader(VERTEX_PATH, FRAGMENT_PATH)
const int WINDOW_WIDTH = 1500;
const float ASPECT_RATIO = 16 / 9.0;
const int WINDOW_HEIGHT = WINDOW_WIDTH * (1 / ASPECT_RATIO);

WindowConfig window_cfg = {
    .name = "Cumin App",
    .aspect_ratio = ASPECT_RATIO,
    .width = WINDOW_WIDTH,
    .height = WINDOW_HEIGHT,
    .floating = true,
};

glm::mat4 projection_matrix =
    glm::ortho(-float(window_cfg.width) / 2, float(window_cfg.width) / 2,
               -float(window_cfg.height) / 2, float(window_cfg.height) / 2);

int main(int argc, char *argv[]) {

  CuminApp app(window_cfg);
  app.on_startup();
  app.run();
  return 0;
}

AppState::AppState()
    : cr(NEW_PRIMITIVE_SHADER, 100, window_cfg.width, window_cfg.height,
         &projection_matrix),
      tr(NEW_PRIMITIVE_SHADER, window_cfg.width, window_cfg.height,
         &projection_matrix),
      qr(NEW_PRIMITIVE_SHADER, window_cfg.width, window_cfg.height,
         &projection_matrix),
      lr(NEW_PRIMITIVE_SHADER, window_cfg.width, window_cfg.height,
         &projection_matrix),
      fr(new Shader("shaders/font_vertex.glsl", "shaders/font_fragment.glsl"),
         "assets/font/ToThePoint.ttf", window_cfg.width, window_cfg.height,
         &projection_matrix) {
  background_color = BLACK;
  animation = 0;
  pixel_square_size = 30;
}

CuminApp::CuminApp(WindowConfig _window_config)
    : App(_window_config), state(AppState()) {}

void CuminApp::clear_screen() {
  Color bg_color = state.background_color;
  glClearColor(bg_color.r, bg_color.g, bg_color.b, bg_color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CuminApp::on_startup() {

  ImGuiIO &io = ImGui::GetIO();
  lc_font = io.Fonts->AddFontFromFileTTF("assets/font/" FONT_ICON_FILE_NAME_LC,
                                         16.0f);

  ci_font = io.Fonts->AddFontFromFileTTF("assets/font/" FONT_ICON_FILE_NAME_CI,
                                         16.0f);

  fa_font = io.Fonts->AddFontFromFileTTF("assets/font/" FONT_ICON_FILE_NAME_FAR,
                                         16.0f);
  ttp_font = io.Fonts->AddFontFromFileTTF("assets/font/ToThePoint.ttf", 16.0f);
}

// In this, the rendering order is important and the rendering function at top
// will override the rendering
void CuminApp::render() {
  clear_screen();

  draw_grid(state.pixel_square_size);

  draw_ui();
}

bool my_tool_active = false;
double value = 0.0f;

void CuminApp::draw_ui() {

  ImGui::SetNextWindowPos(ImVec2(0, window.height - 100));
  ImGui::SetNextWindowSize(ImVec2(window.width, 200));
  ImGui::PushFont(fa_font);
  ImGui::Begin("Transformations", &my_tool_active);
  {
    if (layout_map.find("Transformations") != layout_map.end()) {
      ImVec2 group_size = layout_map["Transformations"];
      ImVec2 window_size = ImGui::GetWindowSize();
      ImGui::SetCursorPosX((window_size.x - group_size.x) / 2);
    }

    ImGui::BeginGroup();
    {
      ImGui::Button("Translate");
      ImGui::SameLine();
      ImGui::Button("Rotation");
      ImGui::SameLine();
      ImGui::Button("Scaling");
      ImGui::SameLine();
      ImGui::Button("Skewing");
      ImGui::SameLine();
      ImGui::Button("Reflection");
    }
    ImGui::EndGroup();

    if (layout_map.find("Transformations") == layout_map.end()) {
      layout_map["Transformations"] = ImGui::GetItemRectMax();
    }
  }
  ImGui::PopFont();
  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(60, window.height - 100));
  ImGui::Begin("Shapes", &my_tool_active);
  {

    ImGui::PushFont(ci_font, 30.0f);
    ImGui::Button(ICON_CI_CIRCLE_LARGE, ImVec2(40, 40));
    ImGui::Button(ICON_CI_PRIMITIVE_SQUARE, ImVec2(40, 40));
    ImGui::Button(ICON_CI_REMOVE, ImVec2(40, 40));
    ImGui::Button(ICON_CI_TRIANGLE_UP, ImVec2(40, 40));
  }
  ImGui::PopFont();
  ImGui::End();

  // Setup Values sidebar which stores matrix and vector
  ImGui::SetNextWindowPos(ImVec2(window.width - 200, 0));
  ImGui::SetNextWindowSize(ImVec2(200, window.height - 100));
  ImGui::PushFont(fa_font);
  ImGui::Begin("Matrix", &my_tool_active);
  ImGui::PopFont();
  {
    ImGui::PushItemWidth(26.0f);
    ImGui::PushFont(ttp_font, 20.0f);

    ImGui::InputDouble("##00", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##01", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##02", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##03", &value);

    ImGui::InputDouble("##10", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##11", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##12", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##13", &value);

    ImGui::InputDouble("##20", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##21", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##22", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##23", &value);

    ImGui::InputDouble("##30", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##31", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##32", &value);
    ImGui::SameLine();
    ImGui::InputDouble("##33", &value);

    ImGui::PopItemWidth();
    ImGui::PopFont();
  }
  ImGui::End();
}

void CuminApp::draw_grid(int pixel_size) {
  // draw column
  for (int y = -window_cfg.height / 2; y < window_cfg.height / 2;
       y += pixel_size) {
    if (y <= pixel_size / 2 && y >= -pixel_size / 2) {
      state.lr.draw2d(-window_cfg.width / 2, y, window_cfg.width, y, 1.0, RED);
      continue;
    }

    state.lr.draw2d(-window_cfg.width / 2, y, window_cfg.width, y, 1.0, WHITE);
  }

  // draw rows
  for (int x = -window_cfg.width / 2; x < window_cfg.width / 2;
       x += pixel_size) {
    if (x <= pixel_size / 2 && x >= -pixel_size / 2) {

      state.lr.draw2d(x, -window_cfg.height / 2, x, window_cfg.height / 2, 1.0,
                      RED);
      continue;
    }

    state.lr.draw2d(x, -window_cfg.height / 2, x, window_cfg.height / 2, 1.0,
                    WHITE);
  }
}

void CuminApp::process_event(AppEvent e) {
  if (glfwGetKey(global.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(global.window, true);
  }
}

void CuminApp::on_update(float ns) {}
