#pragma once

class Background : public View
{
public:
	Background();

	int Display() override;
private:
	vita2d_texture *img_bg;
};

