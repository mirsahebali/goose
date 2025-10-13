#pragma once

#include "draw.h"
#include <utility>

enum class Transformation : uint {
  Translation,
  Rotation,
  Scaling,
  Skewing,
  Reflection,
};

template <typename R> class ShapeObject {

public:
  Vec2 pos;
  Vec2 final_pos;

  Color color;
  bool is_filled;
  uint animation_duration_ms;
  R renderer;

  virtual void push_transformation(std::pair<Transformation, glm::mat3> pair);
  virtual void apply_transformation();
  virtual void draw();

  ShapeObject<R>(Vec2 pos, Color color, bool is_filled);
};

class Line : public ShapeObject<LineRenderer *> {

public:
  Line(Vec2 pos, Color color, LineRenderer *renderer);

  void draw() override;
  void push_transformation(std::pair<Transformation, glm::mat3> pair) override;
  void apply_transformation() override;
};
class Rect : public ShapeObject<QuadRenderer *> {
public:
  Rect(Vec2 pos);
  void draw() override;
  void push_transformation(std::pair<Transformation, glm::mat3> pair) override;
  void apply_transformation() override;
};
class Triangle : public ShapeObject<TriangleRenderer *> {
public:
  void draw() override;
  void push_transformation(std::pair<Transformation, glm::mat3> pair) override;
  void apply_transformation() override;
};
class Circle : public ShapeObject<CircleRenderer *> {
public:
  void draw() override;
  void push_transformation(std::pair<Transformation, glm::mat3> pair) override;
  void apply_transformation() override;
};
class Font : public ShapeObject<FontRenderer *> {
public:
  void draw() override;
  void push_transformation(std::pair<Transformation, glm::mat3> pair) override;
  void apply_transformation() override;
};
