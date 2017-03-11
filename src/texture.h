#pragma once

#include <global_include.h>

#include "shapes.h"

class Texture {
public:
  Texture(unsigned char *addr);
  Texture(const std::string &path);

  int Draw(const Point &pt);
  int DrawResize(const Point &pt1, const Point &pt2);

      static std::map<std::string, vita2d_texture *> textureCache1;
  static std::map<unsigned char *, vita2d_texture *> textureCache2;

private:
  vita2d_texture *texture;
};