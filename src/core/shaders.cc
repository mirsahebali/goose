#include "glad/glad.h"

#include "shaders.h"
#include "window.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

// INFO:
// - There may be multiple similar vertex shaders previously compiled and we
// should probably reuse it
// - Keep that in mind in case of optimization
Shader::Shader(const char *vertex_path, const char *fragment_path) {
  string vertex_code;
  string fragment_code;

  std::ifstream vertex_shader_file;
  std::ifstream fragment_shader_file;

  vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragment_shader_file.exceptions(std::ifstream::failbit |
                                  std::ifstream::badbit);

  try {
    vertex_shader_file.open(vertex_path);
    fragment_shader_file.open(fragment_path);

    std::stringstream vertex_shader_stream, fragment_shader_stream;

    vertex_shader_stream << vertex_shader_file.rdbuf();
    fragment_shader_stream << fragment_shader_file.rdbuf();

    vertex_shader_file.close();
    fragment_shader_file.close();

    vertex_code = vertex_shader_stream.str();
    fragment_code = fragment_shader_stream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_READ_ERROR" << std::endl;
    exit(-1);
  }

  const char *vertex_shader_source_code = vertex_code.c_str();
  const char *fragment_shader_source_code = fragment_code.c_str();

  uint vertex, fragment;
  int success;
  char info_log[512];

  vertex = glCreateShader(GL_VERTEX_SHADER);
  if (vertex == 0) {
    std::cerr << "Failed to create shader!" << std::endl;
    return;
  }
  glShaderSource(vertex, 1, &vertex_shader_source_code, NULL);
  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR) {
    std::cerr << "OpenGL error: " << err << std::endl;
  }

  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, info_log);
    std::cout << "ERROR::COMPILATION::SHADER::VERTEX:\n"
              << info_log << std::endl;
    exit(success);
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragment_shader_source_code, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, info_log);
    std::cout << "ERROR::COMPILATION::SHADER::FRAGMENT:\n"
              << info_log << std::endl;
    exit(success);
  }

  ID = glCreateProgram();

  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, info_log);

    std::cout << "ERROR::LINKING::PROGRAM::" << ID << ":\n"
              << info_log << std::endl;
    exit(success);
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader() {
  cout << "Cleaning up shader id: " << ID << endl;
  glDeleteProgram(ID);
}

void Shader::use() {
  glUseProgram(ID);
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR) {
    printf("OpenGL Error before glUseProgram: %u\n", error);
  }
}
void Shader::set_bool(const string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::set_int(const string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::set_float(const string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_mat4(const string &name, glm::mat4 matrix) {
  uint transformation_location = glGetUniformLocation(ID, name.c_str());
  glUniformMatrix4fv(transformation_location, 1, GL_FALSE,
                     glm::value_ptr(matrix));
}

void Shader::set_vec4(const string &name, glm::vec4 value) {
  uint vec4_location = glGetUniformLocation(ID, name.c_str());
  glUniform4fv(vec4_location, 1, glm::value_ptr(value));
}
