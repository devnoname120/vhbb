#pragma once

#include <memory>
#include <unordered_map>
#include <vita2d.h>
#include "shapes.h"

void DeleteTexture(vita2d_texture* tex);

namespace std {
inline std::shared_ptr<vita2d_texture> make_shared(vita2d_texture* tex) {
    return std::shared_ptr<vita2d_texture>(tex, DeleteTexture);
}
}

class CachingPolicy {
public:
    virtual std::optional<std::shared_ptr<vita2d_texture>> loadCache(const std::string& path) = 0;
    virtual std::optional<std::shared_ptr<vita2d_texture>> loadCache(unsigned char* addr) = 0;

    virtual void saveCache(const std::string& path, std::shared_ptr<vita2d_texture> tex) = 0;
    virtual void saveCache(unsigned char* addr, std::shared_ptr<vita2d_texture> tex) = 0;
};


// Note: can't have virtual static methods in C++ hence why CachingPolicy is not used as base class
class NoCaching {
public:
    static std::optional<std::shared_ptr<vita2d_texture>> loadCache(const std::string& path);
    static std::optional<std::shared_ptr<vita2d_texture>> loadCache(unsigned char* addr);
    static void saveCache(const std::string& path, std::shared_ptr<vita2d_texture> tex);
    static void saveCache(unsigned char* addr, std::shared_ptr<vita2d_texture> tex);
};

class FullCaching {
public:
    static std::optional<std::shared_ptr<vita2d_texture>> loadCache(const std::string& path);
    static std::optional<std::shared_ptr<vita2d_texture>> loadCache(unsigned char* addr);
    static void saveCache(const std::string& path, std::shared_ptr<vita2d_texture> tex);
    static void saveCache(unsigned char* addr, std::shared_ptr<vita2d_texture> tex);

    static std::unordered_map<std::string, std::shared_ptr<vita2d_texture>> textureCache1;
    static std::unordered_map<unsigned char *, std::shared_ptr<vita2d_texture>> textureCache2;
};

template <typename TCachingPolicy>
class Texture {
public:
  enum Status {
      LOADING,
      LOADED,
      FAILED
  };

  Texture(unsigned char *addr);
  Texture(const std::string &path);

  Texture(const Texture& that);
  Texture& operator=(const Texture& that);

  int Draw(const Point &pt);
  int DrawExt(const Point &pt, int alpha);
  int DrawResize(const Point &pt1, const Point &dimensions);
  int DrawCentered(const Point &pt);
  int DrawTint(const Point &pt, unsigned int color);

  unsigned int Height();

  std::shared_ptr<vita2d_texture> texture;

private:
  Status m_status = LOADING;
};

using UncachedTexture = Texture<NoCaching>;
using CachedTexture = Texture<FullCaching>;