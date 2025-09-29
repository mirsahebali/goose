#pragma once

#include "camera.h"
#include "window.h"
#include <glad/glad.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <math.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using vec3 = glm::vec3;
using mat4 = glm::mat4;

void process_input(Window *window);
void mouse_callback(Window *window, double x_pos, double y_pos);
void scroll_callback(Window *window, double x_offset, double y_offset);

typedef struct {
  Camera camera;
  float delta_time;
  float last_frame_time;
  bool first_mouse_enter;
} Global;
