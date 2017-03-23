#pragma once

#include <global_include.h>


class InfoProgress {
public:
    InfoProgress();

    InfoProgress Range(float min, float max);

    float percent();
    void percent(float);

    int speed();
    void speed(int);

    std::string message();
    void message(std::string);
    void message(const char *message);
private:
    float range_min_ = 0;
    float range_max_ = 100;
    std::shared_ptr<float> percent_;
    std::shared_ptr<int> speed_;
    std::shared_ptr<std::string> message_;

    InfoProgress& range(float min, float max);
};
