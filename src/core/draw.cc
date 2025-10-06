#include "glad/glad.h"

#include "stb_truetype.h"

#include "draw.h"

#include "shaders.h"
#include "textures.h"
#include "utils.h"

#include "window.h"

#include <GL/glext.h>
#include <fstream>
#include <glm/ext.hpp>
#include <glm/trigonometric.hpp>
#include <optional>
#include <sys/types.h>
#include <vector>

PrimitiveRenderer::PrimitiveRenderer(Shader *_shader, int _window_width,
                                     int _window_height)
    : shader(_shader), window_height(_window_height),
      window_width(_window_width),
      aspect_ratio(float(_window_width) / _window_height) {}

PrimitiveRenderer::~PrimitiveRenderer() { glDeleteShader(shader->ID); }

LineRenderer::LineRenderer(Shader *_shader, int _window_width,
                           int _window_height)
    : PrimitiveRenderer(_shader, _window_width, _window_height) {

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, nullptr, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void LineRenderer::draw2d(int pixel_start_x, int pixel_start_y, int pixel_end_x,
                          int pixel_end_y, float thickness, Color color) {

  vertex_data[0] = pixel_to_ndc(pixel_start_x, window_width);
  vertex_data[1] = pixel_to_ndc(pixel_start_y, window_height);
  vertex_data[2] = 0.0f;

  vertex_data[3] = pixel_to_ndc(pixel_end_x, window_width);
  vertex_data[4] = pixel_to_ndc(pixel_end_y, window_height);
  vertex_data[5] = 0.0f;

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6, vertex_data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();
  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));

  render_lines();
}

void LineRenderer::draw2d_ndc(float start_x, float start_y, float end_x,
                              float end_y, float thickness, Color color) {

  vertex_data[0] = start_x;
  vertex_data[1] = start_y;
  vertex_data[2] = 0.0f;

  vertex_data[3] = end_x;
  vertex_data[4] = end_y;
  vertex_data[5] = 0.0f;

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6, vertex_data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();
  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));

  render_lines();
}

void LineRenderer::render_lines() {

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

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6, vertex_data);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

LineRenderer::~LineRenderer() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

CircleRenderer::CircleRenderer(Shader *circle_shader, int num_segments,
                               int _window_width, int _window_height)
    : PrimitiveRenderer(circle_shader, _window_width, _window_height),
      num_segments(num_segments), ndc_rendering(false) {
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

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void generateCircleVertices(std::vector<float> &vertices, Vec3 center,
                            float radius, int segments, bool ndc_mode,
                            int window_width, int window_height) {
  vertices[0] = center.x;
  vertices[1] = center.y;
  vertices[2] = center.z;

  int pixel_center_x =
      ndc_mode ? ndc_to_pixel(center.x, window_width) : center.x;
  int pixel_center_y =
      ndc_mode ? ndc_to_pixel(center.y, window_height) : center.y;
  int pixel_radius = ndc_mode ? ndc_to_pixel(radius, window_width) : radius;

  float angle_increment = (2.0f * glm::pi<double>()) / float(segments);
  for (int i = 0; i < segments; ++i) {
    float angle = float(i) * angle_increment;

    float x = pixel_center_x + (pixel_radius * cos(angle));
    float y = pixel_center_y + (pixel_radius * sin(angle));

    uint index = (i + 1) * 3;
    vertices[index] = pixel_to_ndc(x, window_width);
    vertices[index + 1] = pixel_to_ndc(y, window_height);
    vertices[index + 2] = 0.0;
  }
}

void CircleRenderer::draw(Vec3 center, float radius, Color color,
                          bool is_filled) {}

void CircleRenderer::draw2d(int center_x, int center_y, uint radius,
                            Color color, bool is_filled) {

  generateCircleVertices(vertex_data,
                         Vec3{float(center_x), float(center_y), 0.0}, radius,
                         num_segments, false, window_width, window_height);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_data.size() * sizeof(float),
                  vertex_data.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();

  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));
  render_circles(is_filled);
}

void CircleRenderer::draw2d_ndc(float center_x, float center_y, float radius,
                                Color color, bool is_filled) {

  generateCircleVertices(vertex_data, Vec3{center_x, center_y, 0.0}, radius,
                         num_segments, true, window_width, window_height);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_data.size() * sizeof(float),
                  vertex_data.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();

  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));
  render_circles(is_filled);
}

void CircleRenderer::render_circles(bool is_filled) {

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

TriangleRenderer::TriangleRenderer(Shader *triangle_shader, int _window_width,
                                   int _window_height)
    : PrimitiveRenderer(triangle_shader, _window_width, _window_height) {
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

void TriangleRenderer::draw2d_ndc(float v1_x, float v1_y, float v1_z,
                                  float v2_x, float v2_y, float v2_z,
                                  float v3_x, float v3_y, float v3_z,
                                  Color color, bool is_filled) {

  // INFO: here 2, 5, 8 are z coordinates and we haven't decided what to do with
  // it probably gonna use some zplane or zfar for this or some kind of
  // projection matrix manipulation

  vertex_data[0] = v1_x;
  vertex_data[1] = v1_y;
  vertex_data[2] = v1_z;
  vertex_data[3] = v2_x;
  vertex_data[4] = v2_y;
  vertex_data[5] = v2_z;
  vertex_data[6] = v3_x;
  vertex_data[7] = v3_y;
  vertex_data[8] = v3_z;

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferSubData(GL_ARRAY_BUFFER, 0, 9 * sizeof(float), vertex_data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();

  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));
  render_triangles(is_filled);
}

void TriangleRenderer::draw2d(int v1_x, int v1_y, int v1_z, int v2_x, int v2_y,
                              int v2_z, int v3_x, int v3_y, int v3_z,
                              Color color, bool is_filled) {

  // INFO: here 2, 5, 8 are z coordinates and we haven't decided what to do with
  // it probably gonna use some zplane or zfar for this or some kind of
  // projection matrix manipulation

  vertex_data[0] = pixel_to_ndc(v1_x, window_width);
  vertex_data[1] = pixel_to_ndc(v1_y, window_height);
  vertex_data[2] = v1_z;
  vertex_data[3] = pixel_to_ndc(v2_x, window_width);
  vertex_data[4] = pixel_to_ndc(v2_y, window_height);
  vertex_data[5] = v2_z;
  vertex_data[6] = pixel_to_ndc(v3_x, window_width);
  vertex_data[7] = pixel_to_ndc(v3_y, window_height);
  vertex_data[8] = v3_z;

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferSubData(GL_ARRAY_BUFFER, 0, 9 * sizeof(float), vertex_data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();

  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));
  render_triangles(is_filled);
}

void TriangleRenderer::render_triangles(bool is_filled) {

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

QuadRenderer::QuadRenderer(Shader *quad_shader, int _window_width,
                           int _window_height)
    : PrimitiveRenderer(quad_shader, _window_width, _window_height) {

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
void QuadRenderer::draw2d(int pixel_start_x, int pixel_start_y, int pixel_width,
                          int pixel_height, Color color, bool is_filled) {

  vertex_data[0] = pixel_to_ndc(pixel_start_x, window_width);
  vertex_data[1] = pixel_to_ndc(pixel_start_y, window_height);
  vertex_data[2] = 0.0f;
  vertex_data[3] = pixel_to_ndc(pixel_start_x + pixel_width, window_width);
  vertex_data[4] = pixel_to_ndc(pixel_start_y, window_height);
  vertex_data[5] = 0.0f;
  vertex_data[6] = pixel_to_ndc(pixel_start_x + pixel_width, window_width);
  vertex_data[7] = pixel_to_ndc(pixel_start_y + pixel_height, window_height);
  vertex_data[8] = 0.0f;
  vertex_data[9] = pixel_to_ndc(pixel_start_x, window_width);
  vertex_data[10] = pixel_to_ndc(pixel_start_y + pixel_height, window_height);
  vertex_data[11] = 0.0f;

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_data), vertex_data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();
  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));
  render_quad(is_filled);
}

void QuadRenderer::draw2d_ndc(float start_x, float start_y, float width,
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

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_data), vertex_data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  shader->use();
  shader->set_vec4("inputColor", glm::vec4(color.r, color.g, color.b, color.a));
  render_quad(is_filled);
}

void QuadRenderer::render_quad(bool is_filled) {

  if (is_filled) {
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_LINE_LOOP, 0, 4);
  }

  glBindVertexArray(0);
}

QuadRenderer::~QuadRenderer() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

FontRenderer::FontRenderer(Shader *shader, std::string &font_path,
                           int _window_width, int _window_height)
    : PrimitiveRenderer(shader, _window_width, _window_height) {
  std::ifstream input_file_stream(font_path, std::ios::binary);

  input_file_stream.seekg(0, std::ios::end);

  auto &&size = input_file_stream.tellg();

  input_file_stream.seekg(0, std::ios::beg);

  u_int8_t *font_data_buf = new u_int8_t[static_cast<size_t>(size)];

  input_file_stream.read((char *)font_data_buf, size);

  int32_t font_count = stbtt_GetNumberOfFonts(font_data_buf);

  if (font_count == -1)
    std::cerr << "The font file doesn't correspond to valid font data\n";
  else
    std::cout << "The File " << font_path << " contains " << font_count
              << " font(s)\n";

  u_int32_t font_atlas_width = 1024;
  u_int32_t font_atlas_height = 1024;

  u_int8_t *font_atlas_bitmap =
      new u_int8_t[font_atlas_width * font_atlas_height];

  const u_int32_t code_point_of_first_char = 32;
  const u_int32_t chars_to_include_in_font_atlas = 95;

  float font_size = 64.0f;

  stbtt_packedchar packed_chars[chars_to_include_in_font_atlas];
  stbtt_aligned_quad aligned_quads[chars_to_include_in_font_atlas];

  stbtt_pack_context ctx;

  stbtt_PackBegin(&ctx, (unsigned char *)font_atlas_bitmap, font_atlas_width,
                  font_atlas_height, 0, 1, nullptr);

  stbtt_PackFontRange(&ctx, font_data_buf, 0, font_size,
                      code_point_of_first_char, chars_to_include_in_font_atlas,
                      packed_chars);

  stbtt_PackEnd(&ctx);

  for (int i = 0; i < chars_to_include_in_font_atlas; i++) {
    float unused_x, unused_y;

    stbtt_GetPackedQuad(packed_chars, font_atlas_width, font_atlas_height, i,
                        &unused_x, &unused_y, &aligned_quads[i], 0);
  }

  u_int32_t font_atlas_texture_id;

  glGenTextures(1, &font_atlas_texture_id);
  glBindTexture(GL_TEXTURE_2D, font_atlas_texture_id);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, font_atlas_width, font_atlas_height, 0,
               GL_RED, GL_UNSIGNED_BYTE, font_atlas_bitmap);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  delete[] font_atlas_bitmap;
  delete[] font_data_buf;
}

void FontRenderer::draw(std::string &text, int width, int height, int font_size,
                        Color color) {

  for (char ch : text) {
  }
}

void FontRenderer::render_text() {}

FontRenderer::~FontRenderer() {}
