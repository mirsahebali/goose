#include "textures.h"
#include "window.h"
#include <optional>

#include "stb_image.h"
TextureBuilder::TextureBuilder() { glGenTextures(1, &id); }

TextureBuilder *TextureBuilder::set_path(const string &path, bool flip) {
  if (flip) {
    stbi_set_flip_vertically_on_load(true);
  }
  this->data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);
  return this;
}

TextureBuilder *TextureBuilder::set_texture_wrap(TextureWrap s_direction,
                                                 TextureWrap t_direction) {

  texture_wrap_value_s = s_direction;
  texture_wrap_value_t = t_direction;
  return this;
}

TextureBuilder *
TextureBuilder::set_texture_filtering(TextureFilters min_filter,
                                      TextureFilters mag_filter) {
  filter_value_min = min_filter;
  filter_value_mag = mag_filter;
  return this;
}

TextureBuilder *TextureBuilder::set_use_rgba(bool value) {
  this->use_rgba = value;
  return this;
}

optional<Texture> TextureBuilder::build() {
  if (data == NULL) {
    cout << "data was not loaded by stbi" << endl;
    return std::nullopt;
  }

  if (!texture_wrap_value_t) {
    cout << "texture_wrap_value_t is not set" << endl;
    return std::nullopt;
  }

  if (!texture_wrap_value_s) {
    cout << "texture_wrap_value_s is not set" << endl;
    return std::nullopt;
  }

  if (!filter_value_min) {
    cout << "filter_value_min is not set" << endl;
    return std::nullopt;
  }

  if (!filter_value_mag) {
    cout << "filter_value_mag is not set" << endl;
    return std::nullopt;
  }

  Texture texture(id, width, height);

  glBindTexture(GL_TEXTURE_2D, texture.ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  uint(*texture_wrap_value_s));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                  uint(*texture_wrap_value_t));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  uint(*filter_value_min));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  uint(*filter_value_mag));

  glTexImage2D(GL_TEXTURE_2D, 0, use_rgba ? GL_RGBA : GL_RGB, width, height, 0,
               use_rgba ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  return texture;
}

Texture::Texture(uint id) { this->ID = id; }
Texture::Texture(uint id, int width, int height) {
  this->ID = id;
  this->height = height;
  this->width = width;
}

Texture::Texture() {}

void Texture::set_texture_wrap(TextureWrapDirection direction,
                               TextureWrap value) {}

void Texture::set_texture_filtering(TextureFilter name, TextureFilters value) {}

void Texture::activate_and_bind(int texture_index) {
  glActiveTexture(GL_TEXTURE0 + texture_index);
  glBindTexture(GL_TEXTURE_2D, ID);
}
int Texture::get_width() { return this->width; }
int Texture::get_height() { return this->height; }
