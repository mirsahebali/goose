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
