#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {

  glm::vec4 vec(1.0, 0.0f, 0.0f, 1.0);
  glm::mat4 trans = glm::mat4(1.0f);

  trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

  vec = trans * vec;

  cout << "x = " << vec.x << "\ny = " << vec.y << "\nz = " << vec.z << " \n"
       << endl;
  return 0;
}
