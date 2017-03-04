#include "background.h"

extern unsigned char _binary_assets_spr_img_bg_png_start;


Background::Background()
{
	img_bg = vita2d_load_PNG_buffer(&_binary_assets_spr_img_bg_png_start);
}

int Background::Display()
{
	vita2d_draw_texture(img_bg, 0, 0);
	return 0;
}
