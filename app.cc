#include "window.h"
#include <glad/glad.h>
#include <math.h>

#include <GLFW/glfw3.h>

#include <iostream>
using std::cout;
using std::endl;

typedef GLFWwindow Window;

const float ASPECT_RATIO = 16.0 / 9.0;

const uint WINDOW_WIDTH = 1000;
const uint WINDOW_HEIGHT = WINDOW_WIDTH * (1 / ASPECT_RATIO);

void process_input(Window *window);

const char *vertextShaderSourceCode =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
    "}\0";

const char *orangeFragmentShaderSourceCode = "\n#version 330 core\n"
                                             "\nout vec4 FragColor;\n"
                                             "\nin vec4 vertexColor;\n"
                                             "\nvoid main(){\n"
                                             "\n  FragColor = vertexColor;\n"
                                             "}\n";

const char *yellowFragmentShaderSourceCode =
    "#version 330 core                          \n"
    "out vec4 FragColor;                        \n"
    "uniform vec4 ourColor;                     \n"
    "void main(){                               \n"
    "FragColor = ourColor;                      \n"
    "}                                          \n";

const char *vertexGradientShaderSourceCode =
    "\n#version 330 core"
    "\nlayout (location = 0) in vec3 aPos;"
    "\nlayout (location = 1) in vec3 aColor;"
    "\n"
    "\nout vec3 ourColor;"
    "\n"
    "\nvoid main(){"
    "\n  gl_Position = vec4(aPos, 1.0);"
    "\n  ourColor = aColor;"
    "\n}";

const char *gradientFragmentShaderSourceCode =
    "\n#version 330 core "
    "\nout vec4 FragColor;"
    "\nin vec3 ourColor;"
    "\nvoid main(){"
    "\n  FragColor = vec4(ourColor, 1.0);"
    "\n}";

int main(int argc, char *argv[]) {

  Window *window = init_window("Goose", WINDOW_WIDTH, WINDOW_HEIGHT, true);

  // compile and check vertex shaders
  uint vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertex_shader, 1, &vertextShaderSourceCode, NULL);
  glCompileShader(vertex_shader);

  int success;

  char info_log[512];

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
    cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << endl;
    return -1;
  }

  // compile and check orange fragment_shader
  uint orange_fragment_shader;

  orange_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(orange_fragment_shader, 1, &orangeFragmentShaderSourceCode,
                 NULL);
  glCompileShader(orange_fragment_shader);

  success = 1;

  char info_log_frament_shader[512];

  glGetShaderiv(orange_fragment_shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(orange_fragment_shader, 512, NULL,
                       info_log_frament_shader);
    cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
         << info_log_frament_shader << endl;
    return -1;
  }

  // compile and check yellow fragment_shader
  uint yellow_fragment_shader;

  yellow_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(yellow_fragment_shader, 1, &yellowFragmentShaderSourceCode,
                 NULL);
  glCompileShader(yellow_fragment_shader);

  glGetShaderiv(yellow_fragment_shader, GL_COMPILE_STATUS, &success);

  success = 1;

  if (!success) {
    glGetShaderInfoLog(yellow_fragment_shader, 512, NULL,
                       info_log_frament_shader);
    cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
         << info_log_frament_shader << endl;
    return -1;
  }

  success = 1;

  uint gradient_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(gradient_fragment_shader, 1, &gradientFragmentShaderSourceCode,
                 NULL);
  glCompileShader(gradient_fragment_shader);

  glGetShaderiv(gradient_fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(gradient_fragment_shader, 512, NULL,
                       info_log_frament_shader);
    cout << "ERROR::SHADER::FRAGMENT::GRADIENT::COMPILATION_FAILED\n"
         << info_log_frament_shader << endl;
    return -1;
  }

  uint gradient_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(gradient_vertex_shader, 1, &vertexGradientShaderSourceCode,
                 NULL);
  glCompileShader(gradient_vertex_shader);

  glGetShaderiv(gradient_vertex_shader, GL_COMPILE_STATUS, &success);
  success = 1;

  if (!success) {
    glGetShaderInfoLog(gradient_vertex_shader, 512, NULL,
                       info_log_frament_shader);
    cout << "ERROR::SHADER::VERTEX::GRADIENT::COMPILATION_FAILED\n"
         << info_log_frament_shader << endl;
    return -1;
  }

  uint orange_shader_program = glCreateProgram();
  uint yellow_shader_program = glCreateProgram();
  uint gradient_shader_program = glCreateProgram();

  glAttachShader(orange_shader_program, vertex_shader);
  glAttachShader(orange_shader_program, orange_fragment_shader);
  glLinkProgram(orange_shader_program);

  glAttachShader(yellow_shader_program, vertex_shader);
  glAttachShader(yellow_shader_program, yellow_fragment_shader);
  glLinkProgram(yellow_shader_program);
  success = 1;

  glGetProgramiv(orange_shader_program, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(orange_shader_program, 514, NULL, info_log);
    cout << "ERROR::SHADER::PROGRAM::STARTUP\n" << info_log << endl;
    return -1;
  }

  glAttachShader(gradient_shader_program, gradient_vertex_shader);
  glAttachShader(gradient_shader_program, gradient_fragment_shader);
  glLinkProgram(gradient_shader_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(orange_fragment_shader);
  glDeleteShader(yellow_fragment_shader);
  glDeleteShader(gradient_fragment_shader);
  glDeleteShader(gradient_vertex_shader);

  float triangle_1[] = {
      0.5f, 0.5f, 0.0f, // vertex 1
      0.5f, 0.0f, 0.0f, // vertex 2
      0.0f, 0.0f, 0.0f, // vertex 3
  };

  float triangle_2[] = {
      -0.5f, 0.5f, 0.0f, // vertex 1
      -0.5f, 0.0f, 0.0f, // vertex 2
      0.0f,  0.0f, 0.0f, // vertex 3
  };

  float gradient_triangle[] = {
      // position       // color
      0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // bottom right
  };
  // uint VBO, VAO;
  //
  // uint EBO;
  // glGenVertexArrays(1, &VAO);
  // glGenBuffers(1, &VBO);
  // glBindVertexArray(VAO);
  //
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  //
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void
  // *)0); glEnableVertexAttribArray(0);
  //
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

  // uint VBOs[2], VAOs[2];
  //
  // glGenVertexArrays(2, VAOs);
  // glGenBuffers(2, VBOs);
  // // binding first VAO
  // glBindVertexArray(VAOs[0]);
  //
  // // binding first VBO
  // glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_1), triangle_1,
  // GL_STATIC_DRAW);
  //
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void
  // *)0); glEnableVertexAttribArray(0);
  //
  // // binding second VAO
  // glBindVertexArray(VAOs[1]);
  //
  // // binding second VBO
  // glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_2), triangle_2,
  // GL_STATIC_DRAW);
  //
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void
  // *)0); glEnableVertexAttribArray(0);
  //

  uint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(gradient_triangle), gradient_triangle,
               GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  while (!glfwWindowShouldClose(window)) {

    float time = glfwGetTime();
    // input
    process_input(window);

    float green_value = sin(time) / 2.0f / 0.5f;

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(gradient_shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // glUseProgram(orange_shader_program);
    // glBindVertexArray(VAOs[0]);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    //
    // int vertexColorLocation =
    //     glGetUniformLocation(yellow_shader_program, "ourColor");
    // glUseProgram(yellow_shader_program);
    // glUniform4f(vertexColorLocation, 0.0f, green_value, 0.0f, 1.0f);
    //
    // glBindVertexArray(VAOs[1]);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    //
    // glBindVertexArray(0);

    // check and call events and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glDeleteVertexArrays(2, VAOs);
  // glDeleteBuffers(2, VBOs);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(orange_shader_program);
  glDeleteProgram(yellow_shader_program);

  glfwTerminate();
  return 0;
}

void process_input(Window *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
