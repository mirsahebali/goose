#include "shaders.h"
#include "textures.h"
#include "window.h"
#include <glad/glad.h>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <math.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef GLFWwindow Window;

const float ASPECT_RATIO = 16.0 / 9.0;

const uint WINDOW_WIDTH = 1000;
const uint WINDOW_HEIGHT = WINDOW_WIDTH * (1 / ASPECT_RATIO);

void process_input(Window *window);

int main(int argc, char *argv[]) {

  Window *window = init_window("Goose", WINDOW_WIDTH, WINDOW_HEIGHT, true);
  if (window == NULL) {
    cout << "Failed to open window" << endl;
    return -1;
  }
  Shader shader("shaders/vertex_gradient.glsl",
                "shaders/fragment_gradient.glsl");

  float cube_vertex[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  //
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
                                       //
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   //
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    //
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    //
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   //
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
                                       //
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   //
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   //
                                       //
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   //
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
                                       //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  //
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   //
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   //
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
                                       //
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,   //
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f   //
  };
  // int indices[] = {
  //     0, 1, 3, // first triangle
  //     1, 2, 3, // second triangle
  // };

  uint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex), cube_vertex,
               GL_STATIC_DRAW);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  //
  // color attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
  //                       (void *)(6 * sizeof(float)));
  //
  // glEnableVertexAttribArray(2);

  optional<Texture> texture1 =
      TextureBuilder()
          .set_path("assets/container.jpg", false)
          ->set_texture_wrap(TextureWrap::Repeat, TextureWrap::Repeat)
          ->set_texture_filtering(TextureFilters::Nearest,
                                  TextureFilters::Nearest)
          ->build();

  optional<Texture> texture2 =
      TextureBuilder()
          .set_path("assets/awesomeface.png", false)
          ->set_texture_wrap(TextureWrap::Repeat, TextureWrap::Repeat)
          ->set_texture_filtering(TextureFilters::Nearest,
                                  TextureFilters::Nearest)
          ->set_use_rgba(true)
          ->build();

  if (!texture1) {
    cout << "Texture 1 was not built" << endl;
    return -1;
  }

  if (!texture2) {
    cout << "Texture 2 was not build" << endl;
    return -1;
  }

  shader.use();
  shader.set_int("texture1", 0);
  shader.set_int("texture2", 1);

  float mix_value = 0.0;

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  float zoom_value = -3.0f;

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, zoom_value));

  glm::mat4 projection =
      glm::perspective(glm::radians(60.0f), ASPECT_RATIO, 0.1f, 100.0f);

  glEnable(GL_DEPTH_TEST);

  glm::vec3 cube_positions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    //
      glm::vec3(2.0f, 10.0f, -15.0f), //
      glm::vec3(1.5f, -2.2f, -2.5f),  //
      glm::vec3(3.0f, 0.4f, -0.12f),  //
      glm::vec3(7.0f, -10.0f, 7.2f),  //
      glm::vec3(0.5f, 2.5f, -3.0f),   //
      glm::vec3(-2.5f, 2.5f, -3.0f),  //
      glm::vec3(-5.5f, 5.5f, -1.0f),  //
      glm::vec3(-5.0f, -3.5f, -5.0f), //
      glm::vec3(-7.5f, 5.5f, 0.0f),   //
  };

  glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);

  while (!glfwWindowShouldClose(window)) {

    float time = glfwGetTime();

    // input
    process_input(window);

    float green_value = sin(time) / 2.0f / 0.5f;

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);

    shader.use();

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      if (mix_value == 1.0)
        continue;
      mix_value += 0.001f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      if (mix_value == 0.0)
        continue;
      mix_value -= 0.001f;
    }

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_RELEASE) {
      view = glm::translate(view, glm::vec3(0.0f, 0.0f, .25f));
    }

    if ((glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_RELEASE)) {

      view = glm::translate(view, glm::vec3(0.0f, 0.0f, -.25f));
    }

    for (uint i = 0; i < 10; i++) {
      glm::mat4 model(1.0f);

      model = glm::translate(model, cube_positions[i]);
      float angle = 20.0f * i;

      texture1->activate_and_bind(0);
      glm::mat4 trans = glm::mat4(1.0f);
      if (i < 3) {
        model = glm::rotate(model, glm::radians(angle),
                            glm::vec3(1.0f, 0.3f, 0.5f));

        trans = glm::rotate(trans, float(time) * glm::radians(50.0f),
                            glm::vec3(0.0f, 0.0f, 1.0f));
      }

      shader.set_float("mix_value", .4);
      shader.set_mat4("transform", trans);
      shader.set_mat4("view", view);
      shader.set_mat4("model", model);
      shader.set_mat4("projection", projection);

      texture2->activate_and_bind(1);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //
    // shader.set_mat4("transform", trans);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteTextures(1, &texture1->ID);
  glDeleteTextures(1, &texture2->ID);

  glfwTerminate();
  return 0;
}

void process_input(Window *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
