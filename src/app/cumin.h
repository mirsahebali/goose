#pragma once

#include <GLFW/glfw3.h>

#include <X11/X.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "app.h"
#include "draw.h"
#include "window.h"

class AppState {
public:
  int animation;
  LineRenderer line_renderer;
  QuadRenderer quad_renderer;
  CircleRenderer circle_renderer;
  TriangleRenderer triangle_renderer;
  FontRenderer font_renderer;
  Color background_color;
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
  void render() override;
  void on_update(float ns) override;
  void process_event(AppEvent event) override;
  void on_key_event();
  void on_mouse_event();

  void clear_screen();

  void draw_grid(int pixel_size);

  ~CuminApp() override = default;
};

typedef struct GlobalState {
  int current_animation;
} GlobalState;
