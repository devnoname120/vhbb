#include "infoProgress.h"

#include <mutex>
#include <string>


InfoProgress::InfoProgress() :
    percent_(new MutexWrapper<float>("inf_prg_percent", 0)),
    speed_(new MutexWrapper<int>("inf_prg_speed", -1)),
    message_(new MutexWrapper<std::string>("inf_prg_message", std::string()))
{
}

InfoProgress InfoProgress::Range(float min, float max)
{
    return InfoProgress(*this).range(min, max);
}

InfoProgress& InfoProgress::range(float min, float max)
{
    float b_range_min_ = range_min_;

    range_min_ = range_min_ + min * (range_max_ - range_min_) / 100;
    range_max_ = b_range_min_ + (max / 100) * (range_max_ - b_range_min_);

    return *this;
}


float InfoProgress::percent()
{
    std::lock_guard<SceMutex> lock(percent_->mtx);
    return range_min_ + (percent_->value) / 100 * (range_max_ - range_min_);
}


void InfoProgress::percent(float percent)
{
    std::lock_guard<SceMutex> lock(percent_->mtx);
    percent_->value = percent / 100 * (range_max_ - range_min_) + range_min_;
}



int InfoProgress::speed()
{
    std::lock_guard<SceMutex> lock(speed_->mtx);
    return speed_->value;
}

void InfoProgress::speed(int speed)
{
    std::lock_guard<SceMutex> lock(speed_->mtx);
    speed_->value = speed;
}

std::string InfoProgress::message()
{
    std::lock_guard<SceMutex> lock(message_->mtx);
    return message_->value;
}

void InfoProgress::message(std::string message)
{
    std::lock_guard<SceMutex> lock(message_->mtx);
    message_->value = std::move(message);
}

void InfoProgress::message(const char *message)
{
    std::lock_guard<SceMutex> lock(message_->mtx);
    message_->value = std::string(message);
}
