#pragma once

#include <global_include.h>

#include "shapes.h"

class Texture {
public:
	Texture(unsigned char* addr);

	int Draw(const Point &pt);

	static std::map<unsigned char*, vita2d_texture*> textureCache;
private:
	vita2d_texture *texture;
};