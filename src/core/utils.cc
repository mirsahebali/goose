#include "utils.h"
#include <iostream>

void print_vec3(glm::vec3 value) {
  std::cout << "x = " << value.x << "\n";
  std::cout << "y = " << value.y << "\n";
  std::cout << "z = " << value.z << "\n";
}

void print_vec3(std::string name, glm::vec3 value) {
  std::cout << "Value of " << name << std::endl;
  std::cout << "x = " << value.x << "\n";
  std::cout << "y = " << value.y << "\n";
  std::cout << "z = " << value.z << "\n";
}

void print_mat4(std::string name, glm::mat4 value) {

  std::cout << "Value of " << name << std::endl;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << value[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "----------------" << std::endl;
}

void print_mat4(glm::mat4 value) {

  std::cout << "----------------" << std::endl;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << value[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "----------------" << std::endl;
}

void print_circle_vertices(std::string name, std::vector<float> &vertex_data) {

  std::cout << "Value of " << name << std::endl;
  for (int i = 0; i < vertex_data.size(); i += 3) {
    std::cout << "x = " << vertex_data[i] << std::endl;
    std::cout << "y = " << vertex_data[i + 1] << std::endl;
    std::cout << "z = " << vertex_data[i + 2] << std::endl;
  }
}

void print_circle_vertices(std::vector<float> &vertex_data) {

  for (int i = 0; i < vertex_data.size(); i += 3) {
    std::cout << "x = " << vertex_data[i] << std::endl;
    std::cout << "y = " << vertex_data[i + 1] << std::endl;
    std::cout << "z = " << vertex_data[i + 2] << std::endl;
  }
}

float pixel_to_ndc(int pixel_value, int max_pixel_value) {
  return pixel_value / (max_pixel_value / 2.0);
}

int ndc_to_pixel(float ndc, int max_pixel_value) {
  return ndc * (max_pixel_value / 2.0);
}
