#pragma once

#include <global_include.h>

#define FONT_DIR "ux0:/app/VHBB00001/resources/fonts/"

class Font {
public:
	Font(const std::string &path, unsigned int fSize);

	int Draw(const Point &pt, const std::string &text, int color = COLOR_WHITE);
	int DrawCentered(const Rectangle &rect, const std::string &text, int color = COLOR_WHITE);
	
private:
	vita2d_font *font;
	unsigned int size;
};