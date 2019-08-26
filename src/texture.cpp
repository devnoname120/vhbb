#include "texture.h"

#include "debug.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vita2d.h>

// TODO Use polymorphism instead of duplicated class

// <sternmull> you could create the key like this: std::ostringstream s;  s << "@" << static_cast<void*>(address); std::string
// key = s.str(); <sternmull> and have a Texture::init(std::string key) that is used by both constructors.

std::unordered_map<std::string, std::shared_ptr<vita2d_texture>> FullCaching::textureCache1;
std::unordered_map<unsigned char*, std::shared_ptr<vita2d_texture>> FullCaching::textureCache2;

void DeleteTexture(vita2d_texture* tex)
{
    log_printf(DBG_DEBUG, "Destroying texture...");
    vita2d_free_texture(tex);
}

template <typename TCachingPolicy>
Texture<TCachingPolicy>::Texture(const Texture& that)
{
    texture = that.texture;
    m_status = that.m_status;
}

template <typename TCachingPolicy>
Texture<TCachingPolicy>& Texture<TCachingPolicy>::operator=(const Texture& that)
{
    if (this != &that)
    {
        texture = that.texture;
        m_status = that.m_status;
    }
    return *this;
}

template <typename TCachingPolicy>
Texture<TCachingPolicy>::Texture(const std::string& path)
{
//    using TCachingPolicy::loadCache;
//    using TCachingPolicy::saveCache;


    std::optional<std::shared_ptr<vita2d_texture>> tex = TCachingPolicy::loadCache(path);
    if (tex.has_value()) {
        texture = tex.value();
        m_status = LOADED;
        return;
    }

    std::size_t found = path.find_last_of('.');
    std::string extension = path.substr(found + 1);


    // FIXME Support Loading Policy
    texture = std::make_shared(vita2d_load_PNG_file(path.c_str()));
    if (!texture)
        texture = std::make_shared(vita2d_load_JPEG_file(path.c_str()));
    if (!texture)
        texture = std::make_shared(vita2d_load_BMP_file(path.c_str()));

    if (!texture)
    {
        log_printf(DBG_ERROR, "Couldn't load texture %s", path.c_str());
        m_status = FAILED;
        return;
    }

    TCachingPolicy::saveCache(path, texture);
    m_status = LOADED;
}

template <typename TCachingPolicy>
Texture<TCachingPolicy>::Texture(unsigned char* addr)
{
//    using TCachingPolicy::loadCache;
//    using TCachingPolicy::saveCache;

    std::optional<std::shared_ptr<vita2d_texture>> tex = TCachingPolicy::loadCache(addr);
    if (tex.has_value()) {
        texture = tex.value();
        m_status = LOADED;
        return;
    }

    // log_printf(DBG_DEBUG, "Looking for size %d, path: %s", fSize, path.c_str());

//    auto key = addr;
//    if (caching)
//    {
//        if (textureCache2.count(key) >= 1)
//        {
//            texture = textureCache2[key];
//            m_status = LOADED;
//            return;
//        }
//    }

    // FIXME Use loading policy
    // FIXME Support for other image formats, and Status
    texture = std::make_shared(vita2d_load_PNG_buffer(addr));

    TCachingPolicy::saveCache(addr, texture);
    m_status = LOADED;
}

template <typename TCachingPolicy>
int Texture<TCachingPolicy>::Draw(const Point& pt)
{
    // TODO Maybe display some kind of placeholder instead?
    if (m_status != LOADED)
        return -1;

    vita2d_draw_texture(texture.get(), pt.x, pt.y);
    return 0;
}

template <typename TCachingPolicy>
int Texture<TCachingPolicy>::DrawExt(const Point& pt, int alpha)
{
    if (m_status != LOADED)
        return -1;

    vita2d_draw_texture_tint(texture.get(), pt.x, pt.y, RGBA8(255, 255, 255, alpha));
    return 0;
}

template <typename TCachingPolicy>
int Texture<TCachingPolicy>::DrawCentered(const Point& pt)
{
    if (m_status != LOADED)
        return -1;
    // Careful, vita2d_texture_get_width() will crash if vita2d_start_drawing() was not called
    float width = vita2d_texture_get_width(texture.get());
    float height = vita2d_texture_get_height(texture.get());
    Point top_left = Point(pt.x - width / 2, pt.y - height / 2);

    vita2d_draw_texture(texture.get(), top_left.x, top_left.y);
    return 0;
}

// vita2d doesn't have a draw resize function: https://github.com/xerpi/libvita2d/issues/42
template <typename TCachingPolicy>
int Texture<TCachingPolicy>::DrawResize(const Point& pt1, const Point& dimensions)
{
    if (m_status != LOADED)
        return -1;

    // Careful, vita2d_texture_get_width() will crash if vita2d_start_drawing() was not called
    float width = vita2d_texture_get_width(texture.get());
    float height = vita2d_texture_get_height(texture.get());
    if (width == 0 || height == 0)
    {
        vita2d_draw_texture(texture.get(), pt1.x, pt1.y);
        return 0;
    }

    float stretchX = ((float)dimensions.x) / width;
    float stretchY = ((float)dimensions.y) / height;
    vita2d_draw_texture_scale(texture.get(), pt1.x, pt1.y, stretchX, stretchY);

    return 0;
}

template <typename TCachingPolicy>
int Texture<TCachingPolicy>::DrawTint(const Point& pt, unsigned int color)
{
    if (m_status != LOADED)
        return -1;

    vita2d_draw_texture_tint(texture.get(), pt.x, pt.y, color);
    return 0;
}

template <typename TCachingPolicy>
unsigned int Texture<TCachingPolicy>::Height()
{
    return vita2d_texture_get_height(texture.get());
}
std::optional<std::shared_ptr<vita2d_texture>> NoCaching::loadCache(const std::string& path)
{
    return std::nullopt;
}
std::optional<std::shared_ptr<vita2d_texture>> NoCaching::loadCache(unsigned char* addr)
{
    return std::nullopt;
}
void NoCaching::saveCache(const std::string& path, std::shared_ptr<vita2d_texture> tex)
{
}
void NoCaching::saveCache(unsigned char* addr, std::shared_ptr<vita2d_texture> tex)
{
}
std::optional<std::shared_ptr<vita2d_texture>> FullCaching::loadCache(const std::string& path)
{
    const auto& key = path;
    if (textureCache1.count(key) >= 1)
    {
        return textureCache1[key];
    }

    return std::nullopt;
}
std::optional<std::shared_ptr<vita2d_texture>> FullCaching::loadCache(unsigned char* addr)
{
    const auto& key = addr;
    if (textureCache2.count(key) >= 1)
    {
        return textureCache2[key];
    }

    return std::nullopt;
}
void FullCaching::saveCache(const std::string& path, std::shared_ptr<vita2d_texture> tex)
{
    textureCache1[path] = tex;
}
void FullCaching::saveCache(unsigned char* addr, std::shared_ptr<vita2d_texture> tex)
{
    textureCache2[addr] = tex;
}

std::shared_ptr<vita2d_texture> SynchronousLoading::loadTexture(const std::string& path)
{
    auto texture = std::make_shared(vita2d_load_PNG_file(path.c_str()));
    if (!texture)
        texture = std::make_shared(vita2d_load_JPEG_file(path.c_str()));
    if (!texture)
        texture = std::make_shared(vita2d_load_BMP_file(path.c_str()));

    return texture;
}
std::shared_ptr<vita2d_texture> SynchronousLoading::loadTexture(unsigned char* addr)
{
    // FIXME Use loading policy
    // FIXME Support for other image formats, and Status
    auto texture = std::make_shared(vita2d_load_PNG_buffer(addr));
    return texture;
}
std::shared_ptr<vita2d_texture> SynchronousLoading::getTexture()
{
    return std::shared_ptr<vita2d_texture>();
}


template class Texture<NoCaching>;
template class Texture<FullCaching>;

