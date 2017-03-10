#include "listItem.h"

#include <texture.h>
#include <font.h>

extern unsigned char _binary_assets_spr_img_itm_panel_png_start;
extern unsigned char _binary_assets_spr_img_itm_panel_highlight_png_start;


ListItem::ListItem(Homebrew hb) 
	: homebrew(hb), font_25(Font(std::string(FONT_DIR "segoeui.ttf"), 25)), font_35(Font(std::string(FONT_DIR "segoeui.ttf"), 35)),
	img_itm_panel(Texture(&_binary_assets_spr_img_itm_panel_png_start)), img_itm_panel_highlight(Texture(&_binary_assets_spr_img_itm_panel_highlight_png_start))
{
}

int ListItem::Display(int posY, int highlight)
{
	display(posY);
	if (highlight)
		displayHighlight(posY);
	return 0;
}

int ListItem::display(int posY)
{
	img_itm_panel.Draw(Point(ITEM_POSX, posY));
	//dbg_printf(DBG_DEBUG, "Drawing title: %s", homebrew.title.c_str());
	font_35.Draw(Point(ITEM_POSX + 120, posY + 33), homebrew.name);
	font_25.Draw(Point(ITEM_POSX + 120, posY + 76), homebrew.author);
	// font_35.Draw(Point(ITEM_POSX + 120, posY + 33), homebrew.version);
	//font_35.Draw(Point(ITEM_POSX + 120, posY + 33), homebrew.release);
	return 0;
}

int ListItem::displayHighlight(int posY)
{
	img_itm_panel_highlight.Draw(Point(ITEM_POSX, posY));
	return 0;
}

