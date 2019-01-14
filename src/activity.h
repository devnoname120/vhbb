#pragma once

#include <global_include.h>

#include "singleton.h"
#include "Views/View.h"


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
    std::mutex mtx_; 
    std::vector<std::shared_ptr<View>> views_;
    std::vector<std::shared_ptr<View>> views_queue;
};
