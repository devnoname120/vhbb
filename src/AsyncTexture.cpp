#include "AsyncTexture.h"
#include <chrono>

ThreadPool AsyncTexture::tp(4);

// TODO use std::forward
void asyncTextureJob1(std::shared_ptr<bool> alive, std::shared_ptr<std::mutex> mutex,
                     AsyncTexture::Status *status,
                     std::shared_ptr<Texture> texture, unsigned char *addr, bool caching) {
    dbg_printf(DBG_DEBUG, "in thread %x %x", addr, mutex);
    std::lock_guard<std::mutex> lock(*mutex);
    dbg_printf(DBG_DEBUG, "got lock on mutex");

    if (!*alive) {
        dbg_printf(DBG_DEBUG, "alive: %d, expired:%d", alive);
        return;
    }

    texture = std::make_shared<Texture>(addr, caching); // FIXME Handle error
    dbg_printf(DBG_DEBUG, "texture loaded");
    *status = AsyncTexture::READY;
    dbg_printf(DBG_DEBUG, "rready %x", texture);
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

void asyncTextureJob2(std::shared_ptr<bool> alive, std::shared_ptr<std::mutex> mutex,
                     AsyncTexture::Status *status,
                     std::weak_ptr<Texture> texture, const std::string path, bool caching) {

    dbg_printf(DBG_DEBUG, "in thread %x %x");
    std::lock_guard<std::mutex> lock(*mutex);
    dbg_printf(DBG_DEBUG, "got lock on mutex");

    if (!*alive) {
        dbg_printf(DBG_DEBUG, "alive: %d, expired:%d", alive);
        return;
    }

    texture = std::make_shared<Texture>(path, caching); // FIXME Handle error
    dbg_printf(DBG_DEBUG, "texture loaded");
    *status = AsyncTexture::READY;
    dbg_printf(DBG_DEBUG, "rready");
}

AsyncTexture::AsyncTexture(unsigned char *addr, bool caching) {
    dbg_printf(DBG_DEBUG, "status: %d", status_);
    tp.AddJob(std::bind(asyncTextureJob1, alive_, mutex_, &status_, texture_, addr, caching));

}

AsyncTexture::AsyncTexture(const std::string &path, bool caching) {
    dbg_printf(DBG_DEBUG, "status: %d", status_);
    tp.AddJob(std::bind(asyncTextureJob2, alive_, mutex_, &status_, texture_, path, caching));
}

AsyncTexture::~AsyncTexture() {
    std::lock_guard<std::mutex> lock(*mutex_);

    *alive_ = false;
}


int AsyncTexture::Draw(const Point &pt) {
    {
        std::lock_guard<std::mutex> lock(*mutex_);
        if (status_ != READY) {
            return 0;
        }
    }

    return texture_->Draw(pt);
}

int AsyncTexture::DrawExt(const Point &pt, int alpha) {
    {
        std::lock_guard<std::mutex> lock(*mutex_);
        if (status_ != READY) {
            dbg_printf(DBG_DEBUG, "not ready: %d", status_);
            return 0;
        }
    }
    dbg_printf(DBG_DEBUG, "text:%x", texture_);
    dbg_printf(DBG_DEBUG, "tex:%x", texture_->texture);
    int ret = texture_->DrawExt(pt, alpha);
    dbg_printf(DBG_DEBUG, "text:%x", texture_);
    return ret;
}

int AsyncTexture::DrawResize(const Point &pt1, const Point &dimensions) {
    {
        std::lock_guard<std::mutex> lock(*mutex_);
        if (status_ != READY)
            return 0;
    }

    return texture_->DrawResize(pt1, dimensions);
}

int AsyncTexture::DrawTint(const Point &pt, unsigned int color) {
    {
        std::lock_guard<std::mutex> lock(*mutex_);
        if (status_ != READY)
            return 0;
    }

    return texture_->DrawTint(pt, color);
}
