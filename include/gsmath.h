#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

using glm::mat4;
using glm::vec3;

mat4 look_at(vec3 position, vec3 target, vec3 up);
