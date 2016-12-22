#include <global_include.h>

extern unsigned char _binary_assets_spr_img_catbar_png_start;
extern unsigned char _binary_assets_spr_img_catbar_highlight_png_start;
extern unsigned char _binary_assets_spr_img_catbar_sep_png_start;


// categoryList_s from categoryView.h should be modified if the size of this list changes
const Category categoryList[categoryList_s] = {
	NEW,
	APPS,
	GAMES,
	EMULATORS,
	UTILITIES
};

const char* categoryList_name[countof(categoryList)] = {
	"New",
	"Apps",
	"Games",
	"Emulators",
	"Utilities"
};


int CategoryView::touchToCat(const Input &input)
{

	for (int i=0; i < categoryTabs.size(); i++) {
		if (input.TouchInRectangle(Rectangle(Point(categoryTabs[i].minX, CAT_Y), Point(categoryTabs[i].maxX, CAT_Y + CAT_HEIGHT)))) {
			return i;
		}
	}
	return -1;
}

CategoryView::CategoryView()
{
	img_catbar = vita2d_load_PNG_buffer(&_binary_assets_spr_img_catbar_png_start);
	img_catbar_highlight = vita2d_load_PNG_buffer(&_binary_assets_spr_img_catbar_highlight_png_start);
	//dbg_printf(DBG_DEBUG, "pok");	
	img_catbar_sep = vita2d_load_PNG_buffer(&_binary_assets_spr_img_catbar_sep_png_start);
	//dbg_printf(DBG_DEBUG, "lok");	
	font_35 = vita2d_load_font_file(FONT_DIR "segoeui.ttf");

	//dbg_printf(DBG_DEBUG, "ok");	
	selectedCat = NEW;
	//dbg_printf(DBG_DEBUG, "nok");

	int remainingWidth = SCREEN_WIDTH;
	double categoryWidth = SCREEN_WIDTH / countof(categoryList);
	
	categoryTabs.reserve(countof(categoryList));
	listViews.reserve(countof(categoryList));
	for (unsigned int i=0; i < countof(categoryList); i++) {
		categoryTabs.push_back(CategoryTab());
		listViews.push_back(ListView());
	}

	categoryTabs[0].minX = 0;
	categoryTabs[0].maxX = (int)categoryWidth;
	dbg_printf(DBG_DEBUG, "0->maxX=%d", categoryTabs[0].maxX);
	remainingWidth -= (int)categoryWidth;
	dbg_printf(DBG_DEBUG, "remainingWidth=%d", remainingWidth);

	for (unsigned int i=1; i < countof(categoryList); i++) {
		categoryTabs[i].minX = categoryTabs[i-1].maxX + 1;
		categoryTabs[i].maxX = categoryTabs[i].minX + (int)remainingWidth / (countof(categoryList) - i);
		remainingWidth -= (int)categoryTabs[i].maxX + 1 - categoryTabs[i].minX;
		dbg_printf(DBG_DEBUG, "%d->minX=%d", i, categoryTabs[i].minX);
		dbg_printf(DBG_DEBUG, "%d->maxX=%d", i, categoryTabs[i].maxX);
		dbg_printf(DBG_DEBUG, "remainingWidth=%d", remainingWidth);
	}
	
}

int CategoryView::HandleInput(int focus, const Input& input)
{
	if (!focus)
		return 0;

	focus = 1;
	if (input.TouchPressed() && input.TouchInRectangle(Rectangle(Point(CAT_X, CAT_Y), Point(SCREEN_WIDTH, CAT_Y + CAT_HEIGHT)))) {
		int ind = touchToCat(input);
		if (ind < 0) {
			dbg_printf(DBG_WARNING, "Touch in cat bar but couldn't find a matching category");
		} else {
			selectedCat = ind;
		}
	} else {
		if (input.KeyNewPressed(SCE_CTRL_LTRIGGER) && selectedCat > 0) {
			selectedCat--;
			dbg_printf(DBG_DEBUG, "LTRIG, selectedCat: %d", selectedCat);
		}

		if (input.KeyNewPressed(SCE_CTRL_RTRIGGER) && selectedCat < countof(categoryList) - 1) {
			selectedCat++;
			dbg_printf(DBG_DEBUG, "RTRIG, selectedCat: %d", selectedCat);
		}
	}

	if (input.TouchPressed() && !input.TouchInRectangle(Rectangle(Point(0, LIST_MIN_Y), Point(SCREEN_WIDTH, LIST_MAX_Y)))) {
		double touchX;
		double touchY;
		input.TouchCoordinates(&touchX, &touchY);

		dbg_printf(DBG_DEBUG, "%f,%f: Not in listView area, unfocusing...", touchX, touchY);
		// test
		dbg_printf(DBG_DEBUG, "%d,%d : %d,%d  -> listView Area", 0, LIST_MIN_Y, SCREEN_WIDTH, LIST_MAX_Y);
		focus = 0;
	}
		

	// FIXME focus is not a good solution	
	listViews[selectedCat].HandleInput(focus, input);

	return 0;
}

int CategoryView::Display()
{
	listViews[selectedCat].Display();
	
	vita2d_draw_texture(img_catbar, CAT_X, CAT_Y);

	for (unsigned int i=0; i < countof(categoryList); i++) {
		// FIXME Center and set real name
		vita2d_font_draw_text(font_35, categoryTabs[i].minX, CAT_Y + CAT_HEIGHT, COLOR_WHITE, 35, "test"/*categoryList_name[i]*/);
		if (i > 0)
			vita2d_draw_texture(img_catbar_sep, categoryTabs[i].minX, CAT_Y);
	}

	float stretchX = ((float)categoryTabs[selectedCat].maxX - categoryTabs[selectedCat].minX)/(float)vita2d_texture_get_width(img_catbar_highlight);
	float stretchY = ((float)CAT_HEIGHT)/(float)vita2d_texture_get_height(img_catbar_highlight);
	vita2d_draw_texture_scale(img_catbar_highlight, CAT_X + categoryTabs[selectedCat].minX, CAT_Y, stretchX, stretchY);

	return 0;
}

