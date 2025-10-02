#include "camera.h"
#include "gsmath.h"

#include <glm/geometric.hpp>

Camera::Camera(vec3 position, vec3 target, vec3 front, vec3 world_up,
               Projection projection, float fov) {
  this->position = position;
  this->target = target;
  this->projection = projection;

  // setting projection matrix
  switch (projection) {
  case Perspective:
    projection_matrix =
        glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, 100.0f);
    break;
  case Orthographic:
    projection_matrix = glm::ortho(
        -float(WINDOW_HEIGHT) / 2.0f, float(WINDOW_HEIGHT) / 2.0f,
        -float(WINDOW_WIDTH) / 2, -float(WINDOW_WIDTH) / 2, 0.1f, 100.0f);
    break;
  }

  this->front = front;
  this->direction = glm::normalize(position - target);
  this->right = glm::normalize(glm::cross(world_up, this->direction));

  this->world_up = world_up;
  this->up = glm::cross(this->direction, this->right);

  this->fov = fov;
  this->yaw = -90.0f;
  this->pitch = 0.0f;
  this->sensitivity = 0.1f;
  // fov and zoom could be considered same thing
  this->zoom = fov;
  this->speed = 2.5f;
  this->aspect_ratio = 16.0f / 9;
}

void Camera::set_fps_style(bool value) { this->is_fps_style = value; }

void Camera::process_keyboard(Direction dir, float delta_time) {
  float velocity = speed * delta_time;
  switch (dir) {
  case Forward:
    position += glm::normalize(front * velocity);
    break;
  case Backward:
    position -= glm::normalize(front * velocity);
    break;
  case Left:
    position -= glm::normalize(right * velocity);
    break;
  case Right:
    position += glm::normalize(right * velocity);
    break;
  }
  if (is_fps_style)
    position.y = 0;
}
void Camera::process_mouse_movement(float x_offset, float y_offset,
                                    GLboolean contraintPitch) {
  x_offset *= sensitivity;
  y_offset *= sensitivity;

  this->add_yaw(x_offset);
  this->add_pitch(y_offset, true);

  this->update_vectors();
}

void Camera::process_mouse_scroll(float y_offset) { this->add_fov(-y_offset); }

void Camera::update_vectors() {
  this->set_direction(pitch, yaw);
  right = this->get_right();
  up = this->get_view_up();
}

vec3 Camera::get_right() {
  right = glm::normalize(glm::cross(front, world_up));
  return right;
}

vec3 Camera::get_view_up() {
  up = glm::cross(right, front);
  return up;
}

void Camera::add_zoom(float value) {
  zoom += value;
  if (zoom < 1.0f)
    zoom = 1.0f;
  if (zoom > 45.0f)
    zoom = 45.0f;
}

vec3 Camera::get_direction() {
  this->direction = glm::normalize(position - target);
  return this->direction;
}
mat4 Camera::get_view_matrix() const {
  return look_at(position, position + front, world_up);
}

mat4 Camera::get_projection_matrix() const { return this->projection_matrix; }

void Camera::set_projection(Projection projection, float z_near, float z_far) {

  this->projection = projection;
  switch (projection) {
  case Perspective:
    projection_matrix =
        glm::perspective(glm::radians(fov), aspect_ratio, z_near, z_far);
    break;
  case Orthographic:
    projection_matrix = glm::ortho(
        -float(WINDOW_HEIGHT) / 2.0f, float(WINDOW_HEIGHT) / 2.0f,
        -float(WINDOW_WIDTH) / 2, -float(WINDOW_WIDTH) / 2, z_near, z_far);
    break;
  }
}

void Camera::set_direction(float pitch, float yaw) {
  vec3 front;

  front.x = cos(radians(yaw)) * cos(radians(pitch));
  front.y = sin(radians(pitch));
  front.z = sin(radians(yaw)) * cos(radians(pitch));

  this->front = glm::normalize(front);
}

void Camera::add_fov(float angle) {
  fov += angle;

  if (fov > max_fov)
    fov = max_fov;
  if (fov < 1.0f)
    fov = 1.0f;
}

void Camera::add_yaw(float angle) { yaw += angle; }

void Camera::add_pitch(float angle) { this->add_pitch(angle, false); }

void Camera::add_pitch(float angle, bool constrained) {
  pitch += angle;

  if (constrained) {
    if (pitch > max_pitch)
      pitch = max_pitch;
    if (pitch < -max_pitch)
      pitch = -max_pitch;
  }
}

void Camera::reset_camera_pos() {
  this->position = vec3(0.0f, 0.0f, 0.0f);
  this->target = vec3(0.0f, 0.0f, 0.0f);
}
