#include "app.h"
#include "camera.h"
#include "shaders.h"
#include "textures.h"
#include <GLFW/glfw3.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>

float mix_value = 0.0;

mat4 view = mat4(1.0f);
float radius = 10;

vec3 camera_pos = vec3(0.0f, 0.0f, 2.0f);
vec3 world_up = vec3(0.f, 1.f, 0.f);
vec3 camera_front = vec3(0.0f, 0.0f, -1.0f);
vec3 camera_target = vec3(0.0f, 0.0f, 0.0f);

float delta_time = 0.0f;
float last_frame_time = 0.0f;

float last_mouse_x = WINDOW_WIDTH / 2.0;
float last_mouse_y = WINDOW_HEIGHT / 2.0;
bool first_mouse_enter = true;

Camera camera(camera_pos, camera_target, camera_front, world_up,
              Projection::Perspective, 45.0f);

int main(int argc, char *argv[]) {

  Window *window = init_window("Goose", WINDOW_WIDTH, WINDOW_HEIGHT, true);
  if (window == NULL) {
    cout << "Failed to open window" << endl;
    return -1;
  }
  camera.set_fps_style(false);

  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);

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

  mat4 model = mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));

  camera.zoom = -3.0f;

  // translation vectors for cube positions
  vec3 cube_positions[] = {
      vec3(0.0f, 0.0f, 0.0f),    //
      vec3(2.0f, 10.0f, -15.0f), //
      vec3(1.5f, -2.2f, -2.5f),  //
      vec3(3.0f, 0.4f, -0.12f),  //
      vec3(7.0f, -10.0f, 7.2f),  //
      vec3(0.5f, 2.5f, -3.0f),   //
      vec3(-2.5f, 2.5f, -3.0f),  //
      vec3(-5.5f, 5.5f, -1.0f),  //
      vec3(-5.0f, -3.5f, -5.0f), //
      vec3(-7.5f, 5.5f, 0.0f),   //
  };

  while (!glfwWindowShouldClose(window)) {

    float current_time = glfwGetTime();
    delta_time = current_time - last_frame_time;
    last_frame_time = current_time;

    camera.set_projection(Projection::Perspective, 0.1f, 100.0f);
    mat4 projection = camera.get_projection_matrix();

    // input
    process_input(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);

    shader.use();

    for (uint i = 0; i < 10; i++) {
      glm::mat4 model(1.0f);

      model = glm::translate(model, cube_positions[i]);
      float angle = 20.0f * i;

      texture1->activate_and_bind(0);
      glm::mat4 trans = glm::mat4(1.0f);
      if (i < 3) {
        model = glm::rotate(model, glm::radians(angle),
                            glm::vec3(1.0f, 0.3f, 0.5f));

        trans = glm::rotate(trans, float(current_time) * glm::radians(50.0f),
                            glm::vec3(0.0f, 0.0f, 1.0f));
      }

      // view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
      view = camera.get_view_matrix();

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

  if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)) {
    camera.process_keyboard(Direction::Forward, delta_time);
  }

  if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)) {
    camera.process_keyboard(Direction::Left, delta_time);
  }

  if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)) {
    camera.process_keyboard(Direction::Backward, delta_time);
  }

  if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {
    camera.process_keyboard(Direction::Right, delta_time);
  }
}

void mouse_callback(Window *window, double x_pos_in, double y_pos_in) {
  float x_pos = float(x_pos_in);
  float y_pos = float(y_pos_in);

  if (first_mouse_enter) {
    last_mouse_x = x_pos;
    last_mouse_y = y_pos;
    first_mouse_enter = false;
  }

  float x_offset = x_pos - last_mouse_x;
  float y_offset = last_mouse_y - y_pos;
  last_mouse_x = x_pos;
  last_mouse_y = y_pos;

  camera.process_mouse_movement(x_offset, y_offset);
}

void scroll_callback(Window *window, double x_offset, double y_offset) {
  camera.process_mouse_scroll(-float(y_offset));
}
