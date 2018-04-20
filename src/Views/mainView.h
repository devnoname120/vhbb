#pragma once

#include <global_include.h>

#include "View.h"
#include <texture.h>
#include "Views/CategoryView/categoryView.h"
#include "Views/background.h"
#include "Views/statusBar.h"

class MainView : public View
{
public:
	MainView();

	int HandleInput(int focus, const Input& input) override;
	int Display() override;
private:
	Background background;
	StatusBar statusBar;
	CategoryView categoryView;
};

