#include "gsmath.h"
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>

mat4 look_at(vec3 position, vec3 target, vec3 world_up) {
  mat4 rotation_matrix;
  vec3 direction = glm::normalize(position - target);
  vec3 right = glm::normalize(glm::cross(world_up, direction));
  vec3 up = glm::normalize(glm::cross(direction, right));

  rotation_matrix = {
      {right.x, up.x, direction.x, 0},
      {right.y, up.y, direction.y, 0},
      {right.z, up.z, direction.z, 0},
      {0, 0, 0, 1},
  };

  mat4 position_translation_matrix = {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {-position.x, -position.y, -position.z, 1},
  };

  return rotation_matrix * position_translation_matrix;
}
