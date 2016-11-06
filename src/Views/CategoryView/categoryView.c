#include <global_include.h>

extern unsigned char _binary_assets_spr_img_catbar_png_start;

vita2d_texture *img_catbar;

const Category categoryList [] = {
	NEW,
	APPS,
	GAMES,
	EMULATORS,
	UTILITIES
	};

extern int selectedCat;

int initCategoryView()
{
	img_catbar = vita2d_load_PNG_buffer(&_binary_assets_spr_img_catbar_png_start);
	selectedCat = NEW;

	return 0;
}

int handleCategoryViewInput(int focus, Input *input)
{
	if (!focus)
		return 0;

	if (input->pad.buttons & SCE_CTRL_LTRIGGER && selectedCat > 0) {
		selectedCat--;
	}

	if (input->pad.buttons & SCE_CTRL_RTRIGGER && selectedCat < countof(categoryList) - 1) {
		selectedCat++;
	}

	// TODO Handle input->touch
	return 0;
}

int displayCategoryView()
{

	vita2d_draw_texture(img_catbar, CAT_X, CAT_Y);
	// TODO Display category background
	//unsigned int count = countof(categoryList);

	return 0;
}
