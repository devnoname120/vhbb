#pragma once

#include <global_include.h>

#define LIST_BASE_X 62
#define LIST_BASE_Y 115

#define LIST_MIN_Y 79
#define LIST_MAX_Y 543 // This ordinate is included too
#define LIST_RANGE_Y (LIST_MAX_Y - LIST_MIN_Y)
#define LIST_HEIGHT (LIST_RANGE_Y + 1)

// Max speed px/ms above which selected item is no more selected
#define LIST_SELECTION_MAX_SPEED 0.000070


class ListView: public View {
public:
	ListView(std::vector<Homebrew> &homebrews);

	int HandleInput(int focus, const Input& input) override;
	int Display() override;

	
private:
	vita2d_texture *img_catbar;
	vita2d_texture *img_catbar_highlight;
	vita2d_texture *img_catbar_sep;

	vita2d_font *font_35;
	
	std::vector<ListItem> listItems;

	int posY = 0; //!< Position in the list of homebrews if we consider it as a texture
	int selectedItem = -1;
	double scrollSpeed = 0;
	//Homebrew *hblist;
	int count; // FIXME: not needed because we use a vector

	int itemPosY(int i);
	int firstDisplayedItem();
	int lastDisplayedItem();
	int firstFullyDisplayedItem();
	int lastFullyDisplayedItem();
	int coordinateToItem(double coordY);
	int updateScrollSpeed(double &scrollSpeed, unsigned long timeDif);
};
