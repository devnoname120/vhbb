#include "categoryView.h"

#include <screen.h>
#include <database.h>
#include <date.h>

extern unsigned char _binary_assets_spr_img_catbar_png_start;
extern unsigned char _binary_assets_spr_img_catbar_highlight_png_start;
extern unsigned char _binary_assets_spr_img_catbar_sep_png_start;


// categoryList_s from categoryView.h should be modified if the size of this list changes
const Category categoryList[categoryList_s] = {
	NEW,
	GAMES,
	PORTS,
	EMULATORS,
	UTILITIES
};

const char* categoryList_name[countof(categoryList)] = {
	"New",
	"Games",
	"Ports",
	"Emulators",
	"Utilities"
};


int CategoryView::touchToCat(const Input &input)
{

	for (unsigned int i=0; i < categoryTabs.size(); i++) {
		if (input.TouchInRectangle(Rectangle(Point(categoryTabs[i].minX, CAT_Y), Point(categoryTabs[i].maxX, CAT_Y + CAT_HEIGHT)))) {
			return i;
		}
	}
	return -1;
}

CategoryView::CategoryView() :
	font_35(Font(std::string(FONT_DIR "segoeui.ttf"), 35)),
	img_catbar(Texture(&_binary_assets_spr_img_catbar_png_start)),
	img_catbar_highlight(Texture(&_binary_assets_spr_img_catbar_highlight_png_start)),
	img_catbar_sep(Texture(&_binary_assets_spr_img_catbar_sep_png_start))
{
	selectedCat = NEW;

	categoryTabs.reserve(countof(categoryList));
	for (unsigned int i=0; i < countof(categoryList); i++) {
		try {
			auto db = Database::get_instance();
			std::vector<Homebrew> hbs;
			switch (categoryList[i]) {
			case NEW:
				hbs = db->Sort(IsNewer(true));
				break;
			case GAMES:
				hbs = db->Filter(IsCategory("1"));
				break;
			case PORTS:
				hbs = db->Filter(IsCategory("2"));
				break;
			case EMULATORS:
				hbs = db->Filter(IsCategory("5"));
				break;
			case UTILITIES:
				hbs = db->Filter(IsCategory("4"));
				break;
			}
			categoryTabs.push_back(CategoryTab(ListView(hbs)));
		} catch (const std::exception& ex) {
			categoryTabs.push_back(CategoryTab(ListView(std::vector<Homebrew>())));
			dbg_printf(DBG_ERROR, "Couldn't create listViews: %s", ex.what());
		}

	}


	int remainingWidth = SCREEN_WIDTH;
	double categoryWidth = SCREEN_WIDTH / countof(categoryList);

	categoryTabs[0].minX = 0;
	categoryTabs[0].maxX = (int)categoryWidth;
	dbg_printf(DBG_DEBUG, "0->maxX=%d", categoryTabs[0].maxX);
	remainingWidth -= (int)categoryWidth;
	dbg_printf(DBG_DEBUG, "remainingWidth=%d", remainingWidth);

	for (unsigned int i=1; i < countof(categoryList); i++) {
		categoryTabs[i].minX = categoryTabs[i-1].maxX;
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
	categoryTabs.at(selectedCat).listView.HandleInput(focus, input);

	return 0;
}

int CategoryView::Display()
{
	categoryTabs[selectedCat].listView.Display();

	img_catbar.Draw(Point(CAT_X, CAT_Y));

	for (unsigned int i=0; i < countof(categoryList); i++) {
		// FIXME Center and set real name

		font_35.DrawCentered(Rectangle(Point(categoryTabs[i].minX, CAT_Y), Point(categoryTabs[i].maxX, CAT_Y + CAT_HEIGHT)), categoryList_name[i]);
		// font_35.Draw(Point(categoryTabs[i].minX, CAT_Y + CAT_HEIGHT), std::string("Test"));
		// vita2d_font_draw_text(font_35, , COLOR_WHITE, 35, "test"/*categoryList_name[i]*/);
		if (i > 0)
			img_catbar_sep.Draw(Point(categoryTabs[i].minX - 1, CAT_Y));
	}

	img_catbar_highlight.DrawResize(Point(CAT_X + categoryTabs[selectedCat].minX, CAT_Y), Point(categoryTabs[selectedCat].maxX - categoryTabs[selectedCat].minX, CAT_HEIGHT));

	return 0;
}

