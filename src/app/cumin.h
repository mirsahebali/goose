#pragma once

#include "app.h"
#include "draw.h"
#include "window.h"

#include <GLFW/glfw3.h>

#include <X11/X.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <map>

class AppState {
public:
  int animation;
  // Line renderer
  LineRenderer lr;
  // Quad renderer
  QuadRenderer qr;
  // Circle renderer
  CircleRenderer cr;
  // Triangle renderer
  TriangleRenderer tr;
  // Font renderer
  FontRenderer fr;
  Color background_color;
  uint pixel_square_size;

  AppState();
};
enum class AppEvent : u8 {
  Idle,
};

class CuminApp : public App<AppEvent> {
  AppState state;
  // LineRenderer line_renderer;
  // QuadRenderer quad_renderer;
  // CircleRenderer circle_renderer;
  // TriangleRenderer triangle_renderer;

public:
  CuminApp(WindowConfig _window_config);
  ImFont *lc_font;
  ImFont *ci_font;
  ImFont *fa_font;
  ImFont *ttp_font;
  std::map<std::string, ImVec2> layout_map;
  void render() override;
  void on_update(float ns) override;
  void on_startup() override;
  void process_event(AppEvent event) override;
  void on_key_event();
  void on_mouse_event();

  void clear_screen();

  void draw_grid(int pixel_size);
  void draw_ui();
  void draw_center_x_y_axis();

  ~CuminApp() override = default;
};

typedef struct GlobalState {
  int current_animation;
} GlobalState;
