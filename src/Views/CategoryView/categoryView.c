#include <global_include.h>

extern unsigned char _binary_assets_spr_img_catbar_png_start;
extern unsigned char _binary_assets_spr_img_catbar_highlight_png_start;
extern unsigned char _binary_assets_spr_img_catbar_sep_png_start;

vita2d_texture *img_catbar;
vita2d_texture *img_catbar_highlight;
vita2d_texture *img_catbar_sep;

vita2d_font *font_35;

// categoryList_s from categoryView.h should be modified if the size of this list changes
const Category categoryList[categoryList_s] = {
	NEW,
	APPS,
	GAMES,
	EMULATORS,
	UTILITIES
	};

const char* categoryList_name[categoryList_s] = {
	"New",
	"Apps",
	"Games",
	"Emulators",
	"Utilities"
	};

extern int selectedCat;


typedef struct {
	int minX;
	int maxX;
} CategoryView;

CategoryView categoryView[categoryList_s];

int initCategoryView()
{
	img_catbar = vita2d_load_PNG_buffer(&_binary_assets_spr_img_catbar_png_start);
	img_catbar_highlight = vita2d_load_PNG_buffer(&_binary_assets_spr_img_catbar_highlight_png_start);
	img_catbar_sep = vita2d_load_PNG_buffer(&_binary_assets_spr_img_catbar_sep_png_start);
	font_35 = vita2d_load_font_file(FONT_DIR "segoeui.ttf");
	
	selectedCat = NEW;

	int remainingWidth = SCREEN_WIDTH;
	double categoryWidth = SCREEN_WIDTH / categoryList_s;

	categoryView[0].minX = 0;
	categoryView[0].maxX = (int)categoryWidth;
	dbg_printf(DBG_DEBUG, "0->maxX=%d", categoryView[0].maxX);
	remainingWidth -= (int)categoryWidth;
	dbg_printf(DBG_DEBUG, "remainingWidth=%d", remainingWidth);

	for (int i=1; i < categoryList_s; i++) {
		categoryView[i].minX = categoryView[i-1].maxX + 1;
		categoryView[i].maxX = categoryView[i].minX + (int)remainingWidth / (categoryList_s - i);
		remainingWidth -= (int)categoryView[i].maxX + 1 - categoryView[i].minX;
		dbg_printf(DBG_DEBUG, "%d->minX=%d", i, categoryView[i].minX);
		dbg_printf(DBG_DEBUG, "%d->maxX=%d", i, categoryView[i].maxX);
		dbg_printf(DBG_DEBUG, "remainingWidth=%d", remainingWidth);
	}


	return 0;
}

int handleCategoryViewInput(int focus, Input *input)
{
	if (!focus)
		return 0;

	if (keyNewPressed(input, SCE_CTRL_LTRIGGER) && selectedCat > 0) {
		selectedCat--;
	}

	if (keyNewPressed(input, SCE_CTRL_RTRIGGER) && selectedCat < categoryList_s - 1) {
		selectedCat++;
	}

	// TODO Handle input->touch
	return 0;
}

int displayCategoryView()
{

	vita2d_draw_texture(img_catbar, CAT_X, CAT_Y);

	for (int i=0; i < categoryList_s; i++) {
		vita2d_font_draw_text(font_35, categoryView[i].minX, CAT_Y + CAT_HEIGHT, COLOR_WHITE, 35, "test"/*categoryList_name[i]*/);
		if (i > 0)
			vita2d_draw_texture(img_catbar_sep, categoryView[i].minX, CAT_Y);
	}

	float stretchX = ((float)categoryView[selectedCat].maxX - categoryView[selectedCat].minX)/(float)vita2d_texture_get_width(img_catbar_highlight);
	float stretchY = ((float)CAT_HEIGHT)/(float)vita2d_texture_get_height(img_catbar_highlight);
	vita2d_draw_texture_scale(img_catbar_highlight, CAT_X + categoryView[selectedCat].minX, CAT_Y, stretchX, stretchY);
	// TODO Display category background
	//unsigned int count = countof(categoryList);

	return 0;
}
