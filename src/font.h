#pragma once

#include <global_include.h>

#include "shapes.h"

#define FONT_DIR "ux0:/app/VHBB00001/resources/fonts/"

class Font {
public:
	Font(const std::string &path, unsigned int fSize);

	int DrawFromBaseline(const Point &pt, const std::string &text, unsigned int color = COLOR_WHITE,
	                     unsigned int maxWidth = 0, unsigned int maxHeight = 0);
	inline int Draw(const Point &pt, const std::string &text, unsigned int color = COLOR_WHITE,
	                unsigned int maxWidth = 0, unsigned int maxHeight = 0);
	int DrawClip(const Point &pt, const std::string &text, const Rectangle &clipRect, unsigned int color=COLOR_WHITE);
	int DrawCentered(const Rectangle &rect, const std::string &text, unsigned int color=COLOR_WHITE, bool clip=false);
	int DrawCenteredVertical(const Rectangle &rect, const std::string &text,
	                         unsigned int color=COLOR_WHITE, bool clip=false);
	std::string FitString(const std::string &text, int maxWidth);

	static std::unordered_map<std::pair<std::string, unsigned int>, vita2d_font*> fontCache;
private:
	vita2d_font *font;
	unsigned int size;
};