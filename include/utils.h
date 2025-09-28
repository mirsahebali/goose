#pragma once

#define ASSERT_WITH_MSG(expr, msg)                                             \
  do {                                                                         \
    if (!(expr)) {                                                             \
      std::cerr << "Assertion Failed: " << #expr << "\nMessage: " << msg;      \
      std::abort();                                                            \
    }                                                                          \
  } while (false)

typedef enum { SH_X, SH_Y, SH_Z } SheerDirection;

class Vec4 {
  float _x;
  float _y;
  float _z;
  float _w;

public:
  Vec4(float x, float y, float z, float w);
  Vec4(float x, float y, float z);

  Vec4 operator*(float mag);
  Vec4 operator*(Vec4 right);

  void rotate(int deg);
  void translate(int tx, int ty);
  void scale(int sx, int sy);
  void sheer(int sh_value, SheerDirection dir);

  void print();

  float x();
  float y();
  float z();
};

class Vec3 : public Vec4 {

  float _x;
  float _y;
  float _z;

public:
  Vec3(float x, float y, float z);
  Vec3(float value);
};

class Mat4 {
public:
  Mat4(float flat_value);
  Mat4(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4);

  Mat4 operator*(float value);

private:
  Vec4 v1;
  Vec4 v2;
  Vec4 v3;
  Vec4 v4;
};

Mat4 cross(Mat4 left, Mat4 right);
Mat4 operator*(Mat4 left, Mat4 right);
