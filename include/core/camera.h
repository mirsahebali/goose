#pragma once

#include <GL/gl.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

using glm::cos;
using glm::mat4;
using glm::radians;
using glm::sin;
using glm::vec2;
using glm::vec3;

typedef unsigned char u8;

typedef enum : u8 { Perspective, Orthographic } Projection;
typedef enum : u8 { Forward, Backward, Left, Right } Direction;

class Camera {
  vec3 position;
  vec3 up;
  vec3 target;
  vec3 world_up;

  vec3 front;
  vec3 right;
  vec3 direction;
  Projection projection;
  mat4 projection_matrix;

  float fov;
  float yaw;
  float pitch;

  float max_pitch = 89;
  float max_fov = 45;

  float aspect_ratio;
  int window_width;
  int window_height;

  bool is_fps_style = false;

public:
  float speed;
  float sensitivity;
  float zoom;

  Camera(vec3 position, vec3 target, vec3 front, vec3 up, Projection projection,
         float fov, int window_width, int window_height);
  Camera() = default;

  mat4 get_view_matrix() const;
  mat4 get_projection_matrix() const;

  vec3 get_view_up();
  vec3 get_right();
  vec3 get_direction();

  void process_keyboard(Direction dir, float delta_time);

  void process_mouse_movement(float x_offset, float y_offset,
                              GLboolean contraintPitch = true);

  void process_mouse_scroll(float y_offset);

  void set_direction(float pitch, float yaw);
  void set_projection(Projection projection, float z_near, float z_far);
  void set_fps_style(bool value);

  void add_fov(float angle);

  void add_zoom(float value);

  void add_yaw(float angle);

  void add_pitch(float angle);

  void add_pitch(float angle, bool constrained);

  void update_vectors();

  void reset_camera_pos();
};

class Camera2D {
  vec2 position;
  vec2 target;
  vec2 offset;
  float rotation;
  float zoom;

public:
  Camera2D(vec2 position, vec2 target);
  Camera2D(vec2 position, vec2 target, vec2 offset, float zoom, float rotation);
  void set_pos(vec3 pos);
  void set_target(vec3 pos);
  void set_rotate(float angle);
  void set_zoom(float value);

  vec2 get_pos();
  vec2 get_target();
  float get_rotate();
  float get_zoom();
};
