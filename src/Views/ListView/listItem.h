#pragma once

#include <global_include.h>

#include <homebrew.h>
#include <texture.h>
#include <font.h>

#define ITEM_HEIGHT 90
#define ITEM_POSX 62

class ListItem {
public:
	 ListItem(Homebrew hb);

	int Display(int posY, int highlight);

	Homebrew homebrew;
private:
	Texture img_itm_panel;
	Texture img_itm_panel_highlight;

	Texture img_icon_;

	Font font_25;
	Font font_35;


	int display(int posY);
	int displayHighlight(int posY);

};

