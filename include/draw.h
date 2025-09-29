#pragma once

#include "textures.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

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
  int r;
  int g;
  int b;
  int a;
} Color;

// 2D objects
void draw_line2d(Vec2 start_pos, Vec2 end_pos, Color color);

void draw_rectangle2d(Vec2 start_pos, Size2D size, Color color, bool filled);

void draw_circle2d(Vec2 center_pos, int radius, Color color, bool filled);

void draw_triangle2d(Vec2 center_pos, int radius, Color color, bool filled);

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
