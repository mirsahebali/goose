#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <string>
#include <vector>
#define ASSERT_WITH_MSG(expr, msg)                                             \
  do {                                                                         \
    if (!(expr)) {                                                             \
      std::cerr << "Assertion Failed: " << #expr << "\nMessage: " << msg;      \
      std::abort();                                                            \
    }                                                                          \
  } while (false)

typedef enum { SH_X, SH_Y, SH_Z } SheerDirection;

void print_vec3(std::string name, glm::vec3);
void print_vec3(glm::vec3);
void print_mat4(glm::mat4);
void print_mat4(std::string name, glm::mat4);

void print_circle_vertices(std::string name, std::vector<float> &);
void print_circle_vertices(std::vector<float> &);
