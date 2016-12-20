#pragma once

#include <global_include.h>


class ListView: public View {
public:
	ListView();

	int HandleInput(int focus, const Input& input) override;
	int Display() override;

	
private:
	vita2d_texture *img_catbar;
	vita2d_texture *img_catbar_highlight;
	vita2d_texture *img_catbar_sep;

	vita2d_font *font_35;
	
	std::vector<ListItem> listItems;

	int posY = 0;
	int selectedItem = 0;
	int momentum = 0;
	//Homebrew *hblist;
	int count; // FIXME: not needed because we use a vector

	int itemPosY(int i);
	int firstDisplayedItem();
	int lastDisplayedItem();
	int firstFullyDisplayedItem();
	int lastFullyDisplayedItem();
	int coordinateToItem(double coordY);
};
