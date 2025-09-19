#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
using std::cout;
using std::endl;

typedef GLFWwindow Window;

const float ASPECT_RATIO = 16.0 / 9.0;

const uint WINDOW_WIDTH = 1000;
const uint WINDOW_HEIGHT = WINDOW_WIDTH * (1 / ASPECT_RATIO);

void framebuffer_size_callback(Window *window, int width, int height);
void process_input(Window *window);

const char *vertextShaderSourceCode =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *orangeFragmentShaderSourceCode =
    "\n#version 330 core\n"
    "\nout vec4 FragColor;\n"
    "\nvoid main(){\n"
    "\n  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

const char *yellowFragmentShaderSourceCode =
    "#version 330 core                          \n"
    "out vec4 FragColor;                        \n"
    "void main(){                               \n"
    "  FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}                                          \n";

int main(int argc, char *argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  Window *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Goose", NULL, NULL);

  if (window == NULL) {

    cout << "Failed to init window " << endl;
    glfwTerminate();

    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

    cout << "Failed to load GLAD" << endl;
    glfwTerminate();
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

  success = 1;

  if (!success) {
    glGetShaderInfoLog(yellow_fragment_shader, 512, NULL,
                       info_log_frament_shader);
    cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
         << info_log_frament_shader << endl;
    return -1;
  }

  uint orange_shader_program = glCreateProgram();
  uint yellow_shader_program = glCreateProgram();

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

  glDeleteShader(vertex_shader);
  glDeleteShader(orange_fragment_shader);
  glDeleteShader(yellow_fragment_shader);

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

  uint VBOs[2], VAOs[2];

  glGenVertexArrays(2, VAOs);
  glGenBuffers(2, VBOs);
  // binding first VAO
  glBindVertexArray(VAOs[0]);

  // binding first VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_1), triangle_1, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // binding second VAO
  glBindVertexArray(VAOs[1]);

  // binding second VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_2), triangle_2, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    // input
    process_input(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(orange_shader_program);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(yellow_shader_program);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    // check and call events and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  glDeleteProgram(orange_shader_program);
  glDeleteProgram(yellow_shader_program);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(Window *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(Window *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
