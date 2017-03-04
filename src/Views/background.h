#pragma once

#include <global_include.h>

#include "View.h"

class Background : public View
{
public:
	Background();

	int Display() override;
private:
	vita2d_texture *img_bg;
};

