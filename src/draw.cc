
#include "draw.h"
#include "window.h"
#include <glm/ext.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <vector>

LineRenderer::LineRenderer(Shader *shader) : shader(shader) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, nullptr, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  glBindVertexArray(0);
}

void LineRenderer::draw2d(float start_x, float start_y, float end_x,
                          float end_y, float thickness, Color color) {

  vertex_data[0] = start_x;
  vertex_data[1] = start_y;
  vertex_data[2] = 0.0f;
  vertex_data[3] = end_x;
  vertex_data[4] = end_y;
  vertex_data[5] = 0.0f;

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6, vertex_data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();
  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));

  render_lines();
}

void LineRenderer::render_lines() {

  glBindVertexArray(VAO);
  glDrawArrays(GL_LINES, 0, 2);
  glBindVertexArray(0);
}

void LineRenderer::draw(Vec3 start, Vec3 end, float thickness, Color color) {

  vertex_data[0] = start.x;
  vertex_data[1] = start.y;
  vertex_data[2] = start.z;
  vertex_data[3] = end.x;
  vertex_data[4] = end.y;
  vertex_data[5] = end.z;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6, vertex_data);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

LineRenderer::~LineRenderer() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void generateCircleVertices(std::vector<float> &vertices, Vec3 center,
                            float radius, int segments) {
  vertices[0] = center.x;
  vertices[1] = center.y;
  vertices[2] = center.z;

  float angle_increment = (2.0f * glm::pi<double>()) / float(segments);
  for (int i = 0; i < segments; ++i) {
    float angle = float(i) * angle_increment;

    float x = center.x + (radius * cos(angle));
    float y = center.y + (radius * sin(angle));

    uint index = (i + 1) * 3;
    vertices[index] = x;
    vertices[index + 1] = y;
    vertices[index + 2] = 0.0;
  }
}

CircleRenderer::CircleRenderer(Shader *circle_shader, int num_segments)
    : shader(circle_shader), num_segments(num_segments) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  this->vertex_data.reserve((num_segments + 1) * 3);
  this->vertex_data.assign((num_segments + 1) * 3, 0.0f);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void CircleRenderer::draw(Vec3 center, float radius, Color color,
                          bool is_filled) {}

void CircleRenderer::draw2d(float center_x, float center_y, float radius,
                            Color color, bool is_filled) {

  generateCircleVertices(vertex_data, Vec3{center_x, center_y, 0.0}, radius,
                         num_segments);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_data.size() * sizeof(float),
                  vertex_data.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();

  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));
  render_circles(is_filled);
}

void CircleRenderer::render_circles(bool is_filled) {

  glBindVertexArray(VAO);
  if (is_filled) {
    glDrawArrays(GL_TRIANGLE_FAN, 1, num_segments + 1);
  } else {
    glDrawArrays(GL_LINE_LOOP, 1, num_segments);
  }
  glBindVertexArray(0);
}

CircleRenderer::~CircleRenderer() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

TriangleRenderer::TriangleRenderer(Shader *triangle_shader)
    : shader(triangle_shader) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  vertex_data[0] = 0.0f;
  vertex_data[1] = 0.0f;
  vertex_data[2] = 0.0f;
  vertex_data[3] = 0.0f;
  vertex_data[4] = 0.0f;
  vertex_data[5] = 0.0f;
  vertex_data[6] = 0.0f;
  vertex_data[7] = 0.0f;
  vertex_data[8] = 0.0f;

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertex_data,
               GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void TriangleRenderer::draw2d(float v1_x, float v1_y, float v1_z, float v2_x,
                              float v2_y, float v2_z, float v3_x, float v3_y,
                              float v3_z, Color color, bool is_filled) {
  vertex_data[0] = v1_x;
  vertex_data[1] = v1_y;
  vertex_data[2] = v1_z;
  vertex_data[3] = v2_x;
  vertex_data[4] = v2_y;
  vertex_data[5] = v2_z;
  vertex_data[6] = v3_x;
  vertex_data[7] = v3_y;
  vertex_data[8] = v3_z;

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferSubData(GL_ARRAY_BUFFER, 0, 9 * sizeof(float), vertex_data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();

  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));
  render_triangles(is_filled);
}

void TriangleRenderer::render_triangles(bool is_filled) {
  glBindVertexArray(VAO);

  if (is_filled) {
    glDrawArrays(GL_TRIANGLES, 0, 3);
  } else {
    glDrawArrays(GL_LINE_LOOP, 0, 3);
  }

  glBindVertexArray(0);
}

TriangleRenderer::~TriangleRenderer() {
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
}

QuadRenderer::QuadRenderer(Shader *quad_shader) : shader(quad_shader) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  vertex_data[0] = 0.0f;
  vertex_data[1] = 0.0f;
  vertex_data[2] = 0.0f;
  vertex_data[3] = 0.0f;
  vertex_data[4] = 0.0f;
  vertex_data[5] = 0.0f;
  vertex_data[6] = 0.0f;
  vertex_data[7] = 0.0f;
  vertex_data[8] = 0.0f;
  vertex_data[9] = 0.0f;
  vertex_data[10] = 0.0f;
  vertex_data[11] = 0.0f;
  indices[0] = 0;
  indices[1] = 1;
  indices[2] = 3;
  indices[3] = 1;
  indices[4] = 2;
  indices[5] = 3;

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), nullptr, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void QuadRenderer::draw2d(float start_x, float start_y, float width,
                          float height, Color color, bool is_filled) {
  vertex_data[0] = start_x;
  vertex_data[1] = start_y;
  vertex_data[2] = 0.0f;
  vertex_data[3] = start_x + width;
  vertex_data[4] = start_y;
  vertex_data[5] = 0.0f;
  vertex_data[6] = start_x + width;
  vertex_data[7] = start_y + height;
  vertex_data[8] = 0.0f;
  vertex_data[9] = start_x;
  vertex_data[10] = start_y + height;
  vertex_data[11] = 0.0f;

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_data), vertex_data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();
  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));
  render_quad(is_filled);
}

void QuadRenderer::render_quad(bool is_filled) {
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  if (is_filled) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    // TODO: make empty rectangle
  }

  glBindVertexArray(0);
}

QuadRenderer::~QuadRenderer() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}
