extern unsigned char _binary_assets_img_bg_png_start;
vita2d_texture *img_bg;


int initBackground()
{
	img_bg = vita2d_load_PNG_buffer(&_binary_assets_img_bg_png_start);
	return 0;
}

int displayBackground()
{
	vita2d_draw_texture(img_bg, 0, 0);
	return 0;
}