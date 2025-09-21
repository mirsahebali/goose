#include "shaders.h"
#include "window.h"
#include <glad/glad.h>
#include <math.h>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"

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

  float rectangle_vertex[] = {
      // Vertex Pos       // Color          // Texture Coorinates
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
  };
  int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3, // second triangle
  };

  uint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertex), rectangle_vertex,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  //
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));

  glEnableVertexAttribArray(2);

  uint texture1;

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width1, height1, nr_channels1;

  stbi_set_flip_vertically_on_load(true);

  unsigned char *data1 =
      stbi_load("assets/container.jpg", &width1, &height1, &nr_channels1, 0);
  if (data1) {

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    cout << "Failed to load  texture" << endl;
    return -1;
  }

  uint texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width2, height2, nr_channels2;
  unsigned char *data2 =
      stbi_load("assets/awesomeface.png", &width2, &height2, &nr_channels2, 0);

  if (data1) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    cout << "error loading texture 2" << endl;
    return -1;
  }

  stbi_image_free(data1);
  stbi_image_free(data2);

  shader.use();
  glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
  shader.set_int("texture2", 1);

  while (!glfwWindowShouldClose(window)) {

    float time = glfwGetTime();
    // input
    process_input(window);

    float green_value = sin(time) / 2.0f / 0.5f;

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return 0;
}

void process_input(Window *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
