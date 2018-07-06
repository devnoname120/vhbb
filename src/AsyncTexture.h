#pragma once


#include "texture.h"
#include "ThreadPool.h"

class AsyncTexture {
public:
    enum Status {
        ERROR,
        LOADING,
        READY
    };

    AsyncTexture(unsigned char *addr, bool caching = true);
    AsyncTexture(const std::string &path, bool caching = true);

    ~AsyncTexture();

    int Draw(const Point &pt);
    int DrawExt(const Point &pt, int alpha);
    int DrawResize(const Point &pt1, const Point &dimensions);
    int DrawTint(const Point &pt, unsigned int color);

    static ThreadPool tp;
private:
    std::shared_ptr<bool> alive_ = std::make_shared<bool>(true);
    std::shared_ptr<std::mutex> mutex_ = std::make_shared<std::mutex>();

    Status status_ = LOADING;
    std::shared_ptr<Texture> texture_;

    AsyncTexture(const AsyncTexture& that);
    AsyncTexture& operator=(const AsyncTexture& that);
};
