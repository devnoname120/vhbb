#include "categoryView.h"

#include <screen.h>
#include <database.h>
#include <date.h>
#include <activity.h>

extern unsigned char _binary_assets_spr_img_catbar_png_start;
extern unsigned char _binary_assets_spr_img_catbar_highlight_png_start;
extern unsigned char _binary_assets_spr_img_catbar_sep_png_start;
extern unsigned char _binary_assets_spr_img_magnifying_glass_png_start;


// categoryList_s from categoryView.h should be modified if the size of this list changes
const Category categoryList[categoryList_s] = {
	NEW,
	GAMES,
	PORTS,
	EMULATORS,
	UTILITIES,
	SEARCH
};

// set -1 to equally distribute the remaining width
const int categoryList_widths[categoryList_s] = {
	CAT_AUTO_WIDTH,
	CAT_AUTO_WIDTH,
	CAT_AUTO_WIDTH,
	CAT_AUTO_WIDTH,
	CAT_AUTO_WIDTH,
	80
};
const char* categoryList_name[_countof(categoryList)] = {
	"New",
	"Games",
	"Ports",
	"Emulators",
	"Utilities",
	""
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
	font_33(Font(std::string(FONT_DIR "segoeui.ttf"), 33)),
	img_catbar(Texture(&_binary_assets_spr_img_catbar_png_start)),
	img_catbar_highlight(Texture(&_binary_assets_spr_img_catbar_highlight_png_start)),
	img_catbar_sep(Texture(&_binary_assets_spr_img_catbar_sep_png_start)),
	img_magnifying_glass(Texture(&_binary_assets_spr_img_magnifying_glass_png_start))
{
	selectedCat = NEW;

	categoryTabs.reserve(_countof(categoryList));
	for (unsigned int i=0; i < _countof(categoryList); i++) {
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
			case SEARCH:
				break;
			}
			categoryTabs.push_back(CategoryTab(ListView(hbs)));
		} catch (const std::exception& ex) {
			categoryTabs.push_back(CategoryTab(ListView(std::vector<Homebrew>())));
			log_printf(DBG_ERROR, "Couldn't create listViews: %s", ex.what());
		}

	}

	log_printf(DBG_DEBUG, "Done tabs creation");


	int remainingWidth = SCREEN_WIDTH;
	unsigned int countAutoWidth = 0;
	for (unsigned int i=0; i < _countof(categoryList); i++) {
		if (categoryList_widths[i] != CAT_AUTO_WIDTH) {
			remainingWidth -= categoryList_widths[i];
		} else {
			countAutoWidth += 1;
		}
	}

	log_printf(DBG_DEBUG, "countAutoWidth %d", countAutoWidth);

	unsigned int categoryAutoWidth = 0;
	if (remainingWidth > 0 && countAutoWidth > 0) {
		categoryAutoWidth = remainingWidth / countAutoWidth;
		remainingWidth -= categoryAutoWidth * countAutoWidth;
	}

	for (unsigned int i=0; i < _countof(categoryList); i++) {
		if (i == 0) {
			categoryTabs[i].minX = 0;
		} else {
			categoryTabs[i].minX = categoryTabs[i-1].maxX;
		}
		if (categoryList_widths[i] != CAT_AUTO_WIDTH) {
			categoryTabs[i].maxX = categoryTabs[i].minX + categoryList_widths[i];
		} else {
			categoryTabs[i].maxX = categoryTabs[i].minX + categoryAutoWidth;
			if (remainingWidth > 0) {
				categoryTabs[i].maxX += 1;
				remainingWidth -= 1;
			}
		}
		log_printf(DBG_DEBUG, "%d->minX=%d", i, categoryTabs[i].minX);
		log_printf(DBG_DEBUG, "%d->maxX=%d", i, categoryTabs[i].maxX);
		log_printf(DBG_DEBUG, "remainingWidth=%d", remainingWidth);
	}


}

void CategoryView::startSearch() {
	IMEView::openIMEView(&_ime_search_view_result, "Search", _ime_search_view_result.userText);
	log_printf(DBG_DEBUG, "Opened search dialog");
}

int CategoryView::HandleInput(int focus, const Input& input)
{
	if (!focus)
		return 0;

	focus = 1;
	if (input.TouchPressed() && input.TouchInRectangle(Rectangle(Point(CAT_X, CAT_Y), Point(SCREEN_WIDTH, CAT_Y + CAT_HEIGHT)))) {
		int ind = touchToCat(input);
		if (ind < 0) {
			log_printf(DBG_WARNING, "Touch in cat bar but couldn't find a matching category");
		} else {
			if (categoryList[ind] == SEARCH) {
				startSearch();
			} else {
				selectedCat = ind;
			}
		}
	} else {
		if (input.KeyNewPressed(SCE_CTRL_LTRIGGER) && selectedCat > 0) {
			if (categoryList[selectedCat-1] != SEARCH) {
				selectedCat--;
			} else {
				if (selectedCat - 1 > 0) {
					// skip search tab if its not the first one
					selectedCat -= 2;
				}
			}
			log_printf(DBG_DEBUG, "LTRIG, selectedCat: %d", selectedCat);
		}

		if (input.KeyNewPressed(SCE_CTRL_RTRIGGER) && selectedCat < _countof(categoryList) - 1) {
			if (categoryList[selectedCat+1] != SEARCH) {
				selectedCat++;
			} else {
				if (selectedCat + 1 < _countof(categoryList) - 1) {
					// skip search tab if its not the last one
					selectedCat += 2;
				}
			}
			log_printf(DBG_DEBUG, "RTRIG, selectedCat: %d", selectedCat);
		}

		if (input.KeyNewPressed(SCE_CTRL_TRIANGLE)) {
			log_printf(DBG_DEBUG, "TRIANGLE, start search");
			startSearch();
		}
	}

	if (input.TouchPressed() && !input.TouchInRectangle(Rectangle(Point(0, LIST_MIN_Y), Point(SCREEN_WIDTH, LIST_MAX_Y)))) {
		double touchX;
		double touchY;
		input.TouchCoordinates(&touchX, &touchY);

//		log_printf(DBG_DEBUG, "%f,%f: Not in listView area, unfocusing...", touchX, touchY);
		// test
//		log_printf(DBG_DEBUG, "%d,%d : %d,%d  -> listView Area", 0, LIST_MIN_Y, SCREEN_WIDTH, LIST_MAX_Y);
		focus = 0;
	}

	if (_ime_search_view_result.status == IMEVIEW_STATUS_FINISHED) {
		log_printf(DBG_DEBUG, "Processing finished search dialog: \"%s\"", _ime_search_view_result.userText.c_str());
		auto db = Database::get_instance();
		std::vector<Homebrew> hbs;
		hbs = db->Search(SearchQuery(_ime_search_view_result.userText));
		for (unsigned int i=0; i < _countof(categoryList); i++) {
			if (categoryList[i] == SEARCH) {
				categoryTabs[i].listView = ListView(hbs);
				selectedCat = i;
			}
		}
		_ime_search_view_result.status = IMEVIEW_STATUS_NONE;
	}


	// FIXME focus is not a good solution
	categoryTabs.at(selectedCat).listView.HandleInput(focus, input);

	return 0;
}

int CategoryView::Display()
{
	categoryTabs[selectedCat].listView.Display();

	img_catbar.Draw(Point(CAT_X, CAT_Y));

	for (unsigned int i=0; i < _countof(categoryList); i++) {
		if (categoryList_name[i][0] != '\0')
			font_33.DrawCentered(Rectangle(Point(categoryTabs[i].minX, CAT_Y), Point(categoryTabs[i].maxX, CAT_Y + CAT_HEIGHT)), categoryList_name[i]);
		if (categoryList[i] == SEARCH)
			img_magnifying_glass.DrawCentered(Point((categoryTabs[i].minX + categoryTabs[i].maxX)/2.0, CAT_Y + CAT_HEIGHT / 2.0));
		if (i > 0)
			img_catbar_sep.Draw(Point(categoryTabs[i].minX - 1, CAT_Y));
	}

	img_catbar_highlight.DrawResize(Point(CAT_X + categoryTabs[selectedCat].minX, CAT_Y), Point(categoryTabs[selectedCat].maxX - categoryTabs[selectedCat].minX, CAT_HEIGHT));

	return 0;
}

