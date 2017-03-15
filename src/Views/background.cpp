#include "background.h"
#include <texture.h>

extern unsigned char _binary_assets_spr_img_bg_png_start;


Background::Background() :
	img_bg(Texture(&_binary_assets_spr_img_bg_png_start))
{
}

int Background::Display()
{
	img_bg.Draw(Point(0, 0));
	return 0;
}
