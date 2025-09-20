#pragma once

#include <string>

class Shader {
public:
  unsigned int ID;
  Shader(const char *vertex_path, const char *fragment_path);

  void use();

  void set_bool(const std::string &name, bool value) const;
  void set_int(const std::string &name, int value) const;
  void set_float(const std::string &name, float value) const;
};
