#pragma once

#include <memory>
#include <vector>
#include "singleton.h"
#include "Views/View.h"
#include "concurrency.h"


class Activity: public Singleton<Activity>, public View {
friend class Singleton<Activity>;
public:
	~Activity() override;

	int HandleInput(int focus, const Input& input) override;
	int Display() override;

    void AddView(std::shared_ptr<View> view);
    void FlushQueue();
    bool HasActivity();


private:
    SceMutex mtx_ = SceMutex("activity_mtx");
    std::vector<std::shared_ptr<View>> views_;
    std::vector<std::shared_ptr<View>> views_queue;
};
