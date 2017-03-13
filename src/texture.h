#pragma once

#include <global_include.h>

#include "shapes.h"

class Texture {
public:
  Texture(unsigned char *addr);
  Texture(const std::string &path);

  int Draw(const Point &pt);
  int DrawResize(const Point &pt1, const Point &dimensions);

  static std::unordered_map<std::string, vita2d_texture *> textureCache1;
  static std::unordered_map<unsigned char *, vita2d_texture *> textureCache2;

private:
  vita2d_texture *texture;
};