#include <global_include.h>

extern unsigned char _binary_assets_spr_img_itm_panel_png_start;
extern unsigned char _binary_assets_spr_img_itm_panel_highlight_png_start;


ListItem::ListItem()
{
	img_itm_panel = vita2d_load_PNG_buffer(&_binary_assets_spr_img_itm_panel_png_start);
	img_itm_panel_highlight = vita2d_load_PNG_buffer(&_binary_assets_spr_img_itm_panel_highlight_png_start);
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
	vita2d_draw_texture(img_itm_panel, ITEM_POSX, posY);
	return 0;
}

int ListItem::displayHighlight(int posY)
{
	vita2d_draw_texture(img_itm_panel_highlight, ITEM_POSX, posY);
	return 0;
}

