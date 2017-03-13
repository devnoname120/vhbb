#pragma once

#include <global_include.h>

#include "singleton.h"
#include "Views/View.h"


class Activity: public Singleton<Activity>, public View {
friend class Singleton<Activity>;
public:
	~Activity();

	int HandleInput(int focus, const Input& input);
	int Display();

    void AddView(View &view);
    bool HasActivity();


private:
    std::vector<View> views_;
};
