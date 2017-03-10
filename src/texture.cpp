#include "texture.h"

// TODO Use polymorphism instead of duplicated class

std::map<unsigned char*, vita2d_texture*> Texture::textureCache;

Texture::Texture(unsigned char* addr) {
	//dbg_printf(DBG_DEBUG, "Looking for size %d, path: %s", fSize, path.c_str());
	auto key = addr;
	if (textureCache.count(key) >= 1) {
		//dbg_printf(DBG_DEBUG, "Found it in cache");
		texture = textureCache[key];
		return;
	}
	//dbg_printf(DBG_DEBUG, "Storing in cache...");
	texture = vita2d_load_PNG_buffer(addr);
	textureCache[key] = texture;
}

int Texture::Draw(const Point &pt) {
	vita2d_draw_texture(texture, pt.x, pt.y);
	return 0;
}
