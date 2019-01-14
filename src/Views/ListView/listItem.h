#pragma once

#include <global_include.h>

#include <homebrew.h>
#include <texture.h>
#include <font.h>

#define ITEM_HEIGHT 90
#define ITEM_POSX 62

class ListItem {
public:
	explicit ListItem(Homebrew hb);

	int Display(int posY, int highlight, int highlightAlpha);

	Homebrew homebrew;
private:
	Font font_22;
	Font font_32;

	Texture img_itm_panel;
	Texture img_itm_panel_highlight;

	Texture img_icon_;
	
	Texture img_itm_label_game;
	Texture img_itm_label_port;
	Texture img_itm_label_emu;
	Texture img_itm_label_util;
	
	int display(int posY);
	int displayHighlight(int posY, int alpha);

};

