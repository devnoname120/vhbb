#include "texture.h"

// TODO Use polymorphism instead of duplicated class

// <sternmull> you could create the key like this: std::ostringstream s;  s << "@" << static_cast<void*>(address); std::string key = s.str();
// <sternmull> and have a Texture::init(std::string key) that is used by both constructors.
std::unordered_map<std::string, std::shared_ptr<vita2d_texture>> Texture::textureCache1;
std::unordered_map<unsigned char *, std::shared_ptr<vita2d_texture>> Texture::textureCache2;


void DeleteTexture(vita2d_texture* tex)
{
    log_printf(DBG_DEBUG, "Destroying texture...");
	vita2d_free_texture(tex);
}


Texture::Texture(const Texture& that)
{
	texture = that.texture;
	caching_ = that.caching_;
	m_status = that.m_status;
}

Texture& Texture::operator=(const Texture& that)
{
    if (this != &that)
    {
		texture = that.texture;
		caching_ = that.caching_;
        m_status = that.m_status;
    }
    return *this;
}

Texture::Texture(const std::string &path, bool caching) :
	caching_(caching)
{
	const auto &key = path;
	if (caching_) {
		if (textureCache1.count(key) >= 1)
		{
			texture = textureCache1[key];
			m_status = LOADED;
			return;
		}
	}

	std::size_t found = path.find_last_of('.');
	std::string extension = path.substr(found+1);

	texture = std::make_shared(vita2d_load_PNG_file(path.c_str()));
	if (!texture) texture = std::make_shared(vita2d_load_JPEG_file(path.c_str()));
	if (!texture) texture = std::make_shared(vita2d_load_BMP_file(path.c_str()));

	if (!texture) {
        log_printf(DBG_ERROR, "Couldn't load texture %s", path.c_str());
        m_status = FAILED;
		return;
	}


	if (caching_) textureCache1[key] = texture;
	m_status = LOADED;
}

Texture::Texture(unsigned char* addr, bool caching) : caching_(caching)
{
	//log_printf(DBG_DEBUG, "Looking for size %d, path: %s", fSize, path.c_str());


	auto key = addr;
	if (caching) {
		if (textureCache2.count(key) >= 1) {

			texture = textureCache2[key];
			m_status = LOADED;
			return;
		}
	}

	// FIXME Support for other image formats, and Status
	texture = std::make_shared(vita2d_load_PNG_buffer(addr));

	if (caching) textureCache2[key] = texture;
	m_status = LOADED;
}


int Texture::Draw(const Point &pt)
{
    // TODO Maybe display some kind of placeholder instead?
	if (m_status != LOADED)
		return -1;

	vita2d_draw_texture(texture.get(), pt.x, pt.y);
	return 0;
}


int Texture::DrawExt(const Point &pt, int alpha)
{
	if (m_status != LOADED)
		return -1;

	vita2d_draw_texture_tint(texture.get(), pt.x, pt.y, RGBA8(255, 255, 255, alpha));
	return 0;
}

int Texture::DrawCentered(const Point &pt) {
	if (m_status != LOADED)
		return -1;
	// Careful, vita2d_texture_get_width() will crash if vita2d_start_drawing() was not called
	float width = vita2d_texture_get_width(texture.get());
	float height = vita2d_texture_get_height(texture.get());
	Point top_left = Point(pt.x - width/2, pt.y - height/2);

	vita2d_draw_texture(texture.get(), top_left.x, top_left.y);
	return 0;
}

// vita2d doesn't have a draw resize function: https://github.com/xerpi/libvita2d/issues/42
int Texture::DrawResize(const Point &pt1, const Point &dimensions)
{	if (m_status != LOADED)
		return -1;

	// Careful, vita2d_texture_get_width() will crash if vita2d_start_drawing() was not called
	float width = vita2d_texture_get_width(texture.get());
	float height = vita2d_texture_get_height(texture.get());
	if (width == 0 || height == 0) {
		vita2d_draw_texture(texture.get(), pt1.x, pt1.y);
		return 0;
	}


    float stretchX = ((float)dimensions.x) / width;
    float stretchY = ((float)dimensions.y) / height;
    vita2d_draw_texture_scale(texture.get(), pt1.x, pt1.y, stretchX, stretchY);

    return 0;
}

int Texture::DrawTint(const Point &pt, unsigned int color)
{
	if (m_status != LOADED)
		return -1;

	vita2d_draw_texture_tint(texture.get(), pt.x, pt.y, color);
	return 0;
}

