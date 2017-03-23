#include "activity.h"

Activity::~Activity()
{

}


int Activity::HandleInput(int focus, const Input& input)
{
    std::lock_guard<std::mutex> lock(mtx_);

    if (views_.size() > 1) {
        for (auto it = begin(views_), it_last = --end(views_); it != it_last; ) {
            (*it)->HandleInput(0, input);
            if ((*it)->request_destroy) {
                it = views_.erase(it);
            } else {
                ++it;
            }

        }
    } else if (views_.size() == 0) {
        return 0;
    }

    views_.back()->HandleInput(1, input);
    if (views_.back()->request_destroy) views_.erase(views_.end() - 1);
    return 0;
}


int Activity::Display()
{
    std::lock_guard<std::mutex> lock(mtx_);

    if (views_.empty()) return 0;

    for (auto it = begin(views_), it_last = end(views_); it != it_last; ++it) {
        (*it)->Display();
    }

    return 0;
}


void Activity::AddView(std::shared_ptr<View> view)
{
    std::lock_guard<std::mutex> lock(mtx_);

    dbg_printf(DBG_DEBUG, "Adding view");
    views_queue.push_back(view);
}

void Activity::FlushQueue()
{
    std::lock_guard<std::mutex> lock(mtx_);

    std::move(views_queue.begin(), views_queue.end(), std::back_inserter(views_));
    views_queue.erase(views_queue.begin(),views_queue.end());
}

bool Activity::HasActivity()
{
    std::lock_guard<std::mutex> lock(mtx_);

    return !views_.empty();
}
