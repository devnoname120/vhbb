#pragma once



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

	CachedTexture img_itm_panel;
  CachedTexture img_itm_panel_highlight;

  CachedTexture img_icon_;

  CachedTexture img_itm_label_game;
  CachedTexture img_itm_label_port;
  CachedTexture img_itm_label_emu;
  CachedTexture img_itm_label_util;
	
	int display(int posY);
	int displayHighlight(int posY, int alpha);

};

