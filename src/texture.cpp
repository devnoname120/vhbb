#include "texture.h"

// TODO Use polymorphism instead of duplicated class

// <sternmull> you could create the key like this: std::ostringstream s;  s << "@" << static_cast<void*>(address); std::string key = s.str();
// <sternmull> and have a Texture::init(std::string key) that is used by both constructors.
std::unordered_map<std::string, vita2d_texture *> Texture::textureCache1;
std::unordered_map<unsigned char *, vita2d_texture *> Texture::textureCache2;


Texture::Texture(const std::string &path)
{
    //dbg_printf(DBG_DEBUG, "Looking for size %d, path: %s", fSize, path.c_str());
    auto key = path;
    if (textureCache1.count(key) >= 1)
    {
		//dbg_printf(DBG_DEBUG, "Found it in cache");
		texture = textureCache1[key];
		return;
    }
    //dbg_printf(DBG_DEBUG, "Storing in cache...");
	std::size_t found = path.find_last_of(".");
	std::string extension = path.substr(found+1);

	if (extension == "jpeg" || extension == "jpg") {
		texture = vita2d_load_JPEG_file(path.c_str());
	} else if (extension == "png") {
		texture = vita2d_load_PNG_file(path.c_str());
	} else {
		found = path.find_last_of("/");
		std::string filename = path.substr(found+1);
		dbg_printf(DBG_ERROR, "Cannot load texture \"%s\": unknown texture type", filename.c_str());
		return;
	}

	if (texture == nullptr) {
		dbg_printf(DBG_ERROR, "Couldn't load texture %s", path.c_str());
		return;
	}

	textureCache1[key] = texture;
}

Texture::Texture(unsigned char* addr)
{
	//dbg_printf(DBG_DEBUG, "Looking for size %d, path: %s", fSize, path.c_str());
	auto key = addr;
	if (textureCache2.count(key) >= 1) {
		//dbg_printf(DBG_DEBUG, "Found it in cache");
		texture = textureCache2[key];
		return;
	}
	//dbg_printf(DBG_DEBUG, "Storing in cache...");
	texture = vita2d_load_PNG_buffer(addr);
	textureCache2[key] = texture;
}

int Texture::Draw(const Point &pt)
{
	vita2d_draw_texture(texture, pt.x, pt.y);
	return 0;
}

// vita2d doesn't have a draw resize function: https://github.com/xerpi/libvita2d/issues/42
int Texture::DrawResize(const Point &pt1, const Point &pt2)
{
    float stretchX = ((float)pt2.x - pt1.x) / (float)vita2d_texture_get_width(texture);
    float stretchY = ((float)pt2.y - pt1.y) / (float)vita2d_texture_get_height(texture);
    vita2d_draw_texture_scale(texture, pt1.x, pt1.y, stretchX, stretchY);
    return 0;
}
