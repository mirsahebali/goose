#pragma once

#include "shaders.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
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

public:
  LineRenderer(Shader *_shader, int _window_width, int _window_height);

  void draw(Vec3 start, Vec3 end, float thickness, Color color);

  void draw2d(int start_x, int start_y, int end_x, int end_y, float thickness,
              Color color);

  void draw2d_ndc(float start_x, float start_y, float end_x, float end_y,
                  float thickness, Color color);

  void render_lines();

  ~LineRenderer();
};

class CircleRenderer : public PrimitiveRenderer {
  std::vector<float> vertex_data;
  int num_segments;
  bool ndc_rendering;

public:
  CircleRenderer(Shader *shader, int num_segments, int _window_width,
                 int _window_height);

  void draw(Vec3 center, float radius, Color color, bool is_filled);

  void draw2d(int center_x, int center_y, uint radius, Color color,
              bool is_filled);
  void draw2d_ndc(float center_x, float center_y, float radius, Color color,
                  bool is_filled);

  void render_circles(bool is_filled);

  ~CircleRenderer();
};

class TriangleRenderer : public PrimitiveRenderer {
  float vertex_data[9];

public:
  TriangleRenderer(Shader *shader, int _window_width, int _window_height);

  void draw(Vec3 v1, Vec3 v2, Vec3 v3, Color color, bool is_filled);

  void draw2d(int v1_x, int v1_y, int v1_z, int v2_x, int v2_y, int v2_z,
              int v3_x, int v3_y, int v3_z, Color color, bool is_filled);

  void draw2d_ndc(float v1_x, float v1_y, float v1_z, float v2_x, float v2_y,
                  float v2_z, float v3_x, float v3_y, float v3_z, Color color,
                  bool is_filled);

  void render_triangles(bool is_filled);

  ~TriangleRenderer();
};

class QuadRenderer : public PrimitiveRenderer {
  float vertex_data[12];
  uint indices[6];

public:
  QuadRenderer(Shader *shader, int _window_width, int _window_height);

  void draw(Vec3 origin, Vec3 volume, Color color, bool is_filled);

  void draw2d(int start_x, int start_y, int width, int height, Color color,
              bool is_filled);

  void draw2d_ndc(float start_x, float start_y, float width, float height,
                  Color color, bool is_filled);

  void render_quad(bool is_filled);

  ~QuadRenderer();
};

class FontRenderer : public PrimitiveRenderer {
public:
  FontRenderer(Shader *shader, std::string &path, int _window_width,
               int _window_height);
  void draw(std::string &text, int width, int height, int font_size,
            Color color);

  void render_text();

  ~FontRenderer();
};
