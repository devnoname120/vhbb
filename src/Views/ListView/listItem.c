#include <global_include.h>

extern unsigned char _binary_assets_spr_img_itm_panel_png_start;
extern unsigned char _binary_assets_spr_img_itm_panel_highlight_png_start;
vita2d_texture *img_itm_panel;
vita2d_texture *img_itm_panel_highlight;

int initListItem()
{
	img_itm_panel = vita2d_load_PNG_buffer(&_binary_assets_spr_img_itm_panel_png_start);
	img_itm_panel_highlight = vita2d_load_PNG_buffer(&_binary_assets_spr_img_itm_panel_highlight_png_start);
	return 0;
}

int displayListItem(int posY)
{
	vita2d_draw_texture(img_itm_panel, ITEM_POSX, posY);
	return 0;
}

int displayListItemHighlight(int posY)
{
	vita2d_draw_texture(img_itm_panel_highlight, ITEM_POSX, posY);
	return 0;
}

