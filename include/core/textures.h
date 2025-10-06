#pragma once

#include <array>
#include <optional>
#include <string>

#include "window.h"

using std::array;
using std::optional;
using std::string;

enum class TextureWrapDirection { S, T, W };

enum class TextureFilter { Minify, Magnify };

enum class TextureWrap : int {
  Repeat = GL_REPEAT,
  MirroredRepeat = GL_MIRRORED_REPEAT,
  ClampToEdge = GL_CLAMP_TO_EDGE,
  ClampToBorder = GL_CLAMP_TO_BORDER,
};

enum class TextureFilters : int {
  Nearest = GL_NEAREST,
  Linear = GL_LINEAR,
  NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
  LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
  NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
  LinearMipmapLinear = GL_LINEAR_MIPMAP_NEAREST,
};

class Texture {
public:
  uint ID;

  Texture(uint id);
  Texture(uint id, int width, int height);
  Texture();

  void set_texture_wrap(TextureWrapDirection direction, TextureWrap value);
  void set_texture_filtering(TextureFilter name, TextureFilters value);
  void activate_and_bind(int texture_index);
  int get_width();
  int get_height();

private:
  int width;
  int height;
  int nr_channels;
};

class TextureBuilder {
public:
  TextureBuilder();
  TextureBuilder *set_path(const string &data, bool flip);

  TextureBuilder *set_texture_wrap(TextureWrap s_direction,
                                   TextureWrap t_direction);

  TextureBuilder *set_texture_filtering(TextureFilters min_filter,
                                        TextureFilters mag_filter);

  TextureBuilder *set_use_rgba(bool value);

  optional<Texture> build();

private:
  uint id;
  unsigned char *data = NULL;
  optional<TextureWrap> texture_wrap_value_s;
  optional<TextureWrap> texture_wrap_value_t;
  optional<TextureFilters> filter_value_mag;
  optional<TextureFilters> filter_value_min;
  bool set_flip_vertically;
  bool use_rgba = false;
  int width;
  int height;
  int nr_channels;
};
