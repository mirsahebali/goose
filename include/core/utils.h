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

// gets the ndc value and the center of the screen being (0,0) and top right
// being (max_pixel_value/2)
// Eg.: if the total width in 800, the top right is going to be x = 400 and
// bottom left is goint to be x = -400
// usually pass in maximum width/height of the viewport as 'max_pixel_value'
float pixel_to_ndc(int pixel_value, int max_pixel_value);

// gets the pixel value and the center of the screen being (0,0) and top right
// being (max_pixel_value/2) hence converts Eg. for max width 800, value of 0.2f
// will output (800/2) * 0.2f = 80 pixel value
// usually pass in maximum width/height of the viewport as 'max_pixel_value'
int ndc_to_pixel(float ndc, int max_pixel_value);
