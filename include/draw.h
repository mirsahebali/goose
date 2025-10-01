#pragma once

#include "shaders.h"
#include "textures.h"
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

class LineRenderer {

private:
  Shader *shader;
  uint VAO, VBO, EBO;
  float vertex_data[6];
  // vertex data will be like
  // start_x, start_y, start_z, end_x, end_y, end_z

public:
  LineRenderer(Shader *shader);

  void draw(Vec3 start, Vec3 end, float thickness, Color color);
  void draw(int start_x, int start_y, int end_x, int end_y, float thickness,
            Color color);

  void draw2d(float start_x, float start_y, float end_x, float end_y,
              float thickness, Color color);

  void render_lines();

  ~LineRenderer();
};

class CircleRenderer {
  Shader *shader;
  std::vector<float> vertex_data;
  uint VAO, VBO, EBO;
  int num_segments;

public:
  CircleRenderer(Shader *shader, int num_segments);

  void draw(Vec3 center, float radius, Color color, bool is_filled);

  void draw2d(float center_x, float center_y, float radius, Color color,
              bool is_filled);

  void render_circles(bool is_filled);

  ~CircleRenderer();
};

class TriangleRenderer {
  Shader *shader;
  float vertex_data[9];
  uint VAO, VBO, EBO;

public:
  TriangleRenderer(Shader *shader);

  void draw(Vec3 v1, Vec3 v2, Vec3 v3, Color color, bool is_filled);

  void draw2d(float v1_x, float v1_y, float v1_z, float v2_x, float v2_y,
              float v2_z, float v3_x, float v3_y, float v3_z, Color color,
              bool is_filled);

  void render_triangles(bool is_filled);

  ~TriangleRenderer();
};

class QuadRenderer {
  Shader *shader;
  float vertex_data[12];
  uint indices[6];
  uint VAO, VBO, EBO;

public:
  QuadRenderer(Shader *shader);

  void draw(Vec3 origin, Vec3 volume, Color color, bool is_filled);

  void draw2d(float start_x, float start_y, float width, float height,
              Color color, bool is_filled);

  void render_quad(bool is_filled);

  ~QuadRenderer();
};

// 2D objects
void draw_line2d(float start_pos_x, float start_pos_y, float end_pos_y,
                 float end_pos_x, Color color);

void draw_line2d(Vec2 start_pos, Vec2 end_pos, Color color);

void draw_rectangle2d(float start_pos_x, float start_pos_y, float end_pos_x,
                      float end_pos_y, float width, float height, Color color,
                      bool filled);
void draw_rectangle2d(Vec2 start_pos, Size2D size, Color color, bool filled);

void draw_circle2d(float center_x, float center_y, int radius, Color color,
                   bool filled);
void draw_circle2d(Vec2 center_pos, int radius, Color color, bool filled);

void draw_triangle2d(Vec2 v1, Vec2 v2, Vec2 v3, Color color, bool filled);

// with texture
void draw_line2d_texture(Texture *texture, Vec2 start_pos, Vec2 end_pos,
                         Color color);

void draw_rectangle2d_texture(Texture *texture, Vec2 start_pos, Size2D size,
                              Color color, bool filled);

void draw_circle2d_texture(Texture *texture, Vec2 center_pos, int radius,
                           Color color, bool filled);

void draw_triangle2d_texture(Texture *texture, Vec2 center_pos, int radius,
                             Color color, bool filled);

// 3D objects
void draw_cuboid(Vec3 start_pos, Size3D volume, Color color, bool filled);

void draw_sphere(Vec3 start_pos, int radius, Color color, bool filled);

void draw_line3d(Vec3 start_pos, Vec3 end_pos, Color color);

void draw_rectangle3d(Vec3 start_pos, Size2D size, Color color, bool filled);

void draw_circle3d(Vec3 center_pos, int radius, Color color, bool filled);

void draw_triangle3d(Vec3 center_pos, int radius, Color color, bool filled);
