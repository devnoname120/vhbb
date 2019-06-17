#pragma once


#include <unordered_map>
#include "shapes.h"
#include "macros.h"

#define FONT_DIR VHBB_RESOURCES "/fonts/"

using StringUIntPair = std::pair<std::string, unsigned int>;

// Required because there is not default implementation for hashing std::pair<std::string, unsigned int>
auto pair_hash = [](const StringUIntPair& v){
	return std::hash<std::string>()(v.first) * 31 + std::hash<unsigned int>()(v.second);
};

using My_unordered_map = std::unordered_map<StringUIntPair, vita2d_font*, decltype(pair_hash)>;

class Font {
public:
	Font(const std::string &path, unsigned int fSize);

	int Draw(const Point &pt, const std::string &text, unsigned int color = COLOR_WHITE,
	         unsigned int maxWidth = 0, unsigned int maxHeight = 0);
	
	int DrawClip(const Point &pt, const std::string &text, const Rectangle &clipRect, unsigned int color=COLOR_WHITE);
	int DrawCentered(const Rectangle &rect, const std::string &text, unsigned int color=COLOR_WHITE, bool clip=false);
	int DrawCenteredVertical(const Rectangle &rect, const std::string &text,
	                         unsigned int color=COLOR_WHITE, bool clip=false);
	std::string FitString(const std::string &text, int maxWidth);

	static My_unordered_map fontCache;
private:
	vita2d_font *font;
	unsigned int size;
};
