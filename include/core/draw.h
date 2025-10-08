#pragma once

#include "shaders.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <map>
#include <vector>

typedef struct {
  float x;
  float y;
} Vec2;

typedef struct {
  float x;
  float y;
  float z;
} Vec3;

typedef struct {
  float width;
  float height;
} Size2D;

typedef struct {
  float width;
  float height;
  float depth;
} Size3D;

typedef struct {
  float r;
  float g;
  float b;
  float a;
} Color;

typedef struct PrimitiveDrawing {
  Shader shader;
  uint VAO;
  uint VBO;
  uint EBO;
} DrawingData;

typedef struct Character {
  uint texture_id;
  glm::ivec2 size;
  glm::ivec2 bearing;
  uint advance;

} Character;

class PrimitiveRenderer {
protected:
  Shader *shader;
  uint VAO, VBO, EBO;
  bool ndc_rendering;
  int window_width;
  int window_height;
  float aspect_ratio;

public:
  PrimitiveRenderer(Shader *_shader, int _window_width, int _window_height);
  virtual ~PrimitiveRenderer();
};

class LineRenderer : public PrimitiveRenderer {

private:
  // vertex data will be in format:
  // start_x, start_y, start_z, end_x, end_y, end_z
  float vertex_data[6];

  void render_lines();

public:
  LineRenderer(Shader *_shader, int _window_width, int _window_height);

  void draw(Vec3 start, Vec3 end, float thickness, Color color);

  void draw2d(int start_x, int start_y, int end_x, int end_y, float thickness,
              Color color);

  void draw2d_ndc(float start_x, float start_y, float end_x, float end_y,
                  float thickness, Color color);

  ~LineRenderer();
};

class CircleRenderer : public PrimitiveRenderer {
  std::vector<float> vertex_data;
  int num_segments;
  bool ndc_rendering;

  void render_circles(bool is_filled);

public:
  CircleRenderer(Shader *shader, int num_segments, int _window_width,
                 int _window_height);

  void draw(Vec3 center, float radius, Color color, bool is_filled);

  void draw2d(int center_x, int center_y, uint radius, Color color,
              bool is_filled);
  void draw2d_ndc(float center_x, float center_y, float radius, Color color,
                  bool is_filled);

  ~CircleRenderer();
};

class TriangleRenderer : public PrimitiveRenderer {
  float vertex_data[9];
  void render_triangles(bool is_filled);

public:
  TriangleRenderer(Shader *shader, int _window_width, int _window_height);

  void draw(Vec3 v1, Vec3 v2, Vec3 v3, Color color, bool is_filled);

  void draw2d(int v1_x, int v1_y, int v1_z, int v2_x, int v2_y, int v2_z,
              int v3_x, int v3_y, int v3_z, Color color, bool is_filled);

  void draw2d_ndc(float v1_x, float v1_y, float v1_z, float v2_x, float v2_y,
                  float v2_z, float v3_x, float v3_y, float v3_z, Color color,
                  bool is_filled);

  ~TriangleRenderer();
};

class QuadRenderer : public PrimitiveRenderer {
  float vertex_data[12];
  uint indices[6];
  void render_quad(bool is_filled);

public:
  QuadRenderer(Shader *shader, int _window_width, int _window_height);

  void draw(Vec3 origin, Vec3 volume, Color color, bool is_filled);

  void draw2d(int start_x, int start_y, int width, int height, Color color,
              bool is_filled);

  void draw2d_ndc(float start_x, float start_y, float width, float height,
                  Color color, bool is_filled);

  ~QuadRenderer();
};

typedef struct {
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 texture_coord;
} FontVertexData;

class FontRenderer : public PrimitiveRenderer {
  std::vector<FontVertexData> vertices_data;
  std::map<char, Character> characters;
  uint texture_id;

  void render_text();

public:
  FontRenderer(Shader *shader, const char *path, int _window_width,
               int _window_height);

  void draw(std::string text, int x, int y, float scale, Color color);

  void draw_ndc(std::string text, float x_pos, float y_pos, int width,
                int height, float scale, Color color);

  ~FontRenderer();
};
