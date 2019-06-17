#include <string>
#include "infoProgress.h"

InfoProgress::InfoProgress() :
    percent_(new float(0)),
    speed_(new int(-1)),
    message_(new std::string())
{
}

InfoProgress InfoProgress::Range(float min, float max)
{
    return InfoProgress(*this).range(min, max);
}

InfoProgress& InfoProgress::range(float min, float max)
{
    float b_range_min_ = range_min_;

    range_min_ = range_min_ + min*(range_max_ - range_min_)/100;
    range_max_ = b_range_min_ + (max/100)*(range_max_ - b_range_min_);

    return *this;
}


float InfoProgress::percent()
{
    return range_min_ + *percent_ /100 *(range_max_ - range_min_);
}


void InfoProgress::percent(float percent)
{
    *percent_ = percent /100 *(range_max_ - range_min_) + range_min_;
}



int InfoProgress::speed()
{
    return *speed_;
}

void InfoProgress::speed(int speed)
{
    *speed_ = speed;
}

std::string InfoProgress::message()
{
    return *message_;
}

void InfoProgress::message(std::string message)
{
    *message_ = std::move(message);
}

void InfoProgress::message(const char *message)
{
    *message_ = std::string(message);
}


