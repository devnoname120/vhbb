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
	categoryTabs.reserve(_countof(categoryList));
	for (unsigned int i=0; i < _countof(categoryList); i++) {
		try {
			auto db = Database::get_instance();
			std::vector<Homebrew> hbs;
			ListView *lv;
			switch (categoryList[i]) {
			case NEW:
				lv = new ListView(db->Sort(IsNewer(true)));
				break;
			case GAMES:
				lv = new ListView(db->Filter(IsCategory("1")));
				break;
			case PORTS:
				lv = new ListView(db->Filter(IsCategory("2")));
				break;
			case EMULATORS:
				lv = new ListView(db->Filter(IsCategory("5")));
				break;
			case UTILITIES:
				lv = new ListView(db->Filter(IsCategory("4")));
				break;
			case SEARCH:
				lv = new SearchView();
				break;
			}
			categoryTabs.emplace_back(lv);
		} catch (const std::exception& ex) {
			categoryTabs.emplace_back(new ListView(std::vector<Homebrew>()));
			log_printf(DBG_ERROR, "Couldn't create listViews %i: %s", i, ex.what());
		}

	}
	log_printf(DBG_DEBUG, "Done tabs creation");

	selectCat(NEW);

	// TODO Create a new container for a list of CategoryTab and move all this logic into it
	int remainingWidth = SCREEN_WIDTH;
	unsigned int countAutoWidth = 0;
	for (const int &categoryList_width : categoryList_widths) {
		if (categoryList_width != CAT_AUTO_WIDTH) {
			remainingWidth -= categoryList_width;
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

CategoryView::~CategoryView() = default;

void CategoryView::selectCat(unsigned int cat) {
	log_printf(DBG_DEBUG, "selectCat(unsigned %i)", cat);
	if (cat != selectedCat) {
		log_printf(DBG_DEBUG, "selectCat(unsigned) %i->SignalDeselected()", selectedCat);
		categoryTabs[selectedCat].listView->SignalDeselected();
	}
	selectedCat = cat;
	log_printf(DBG_DEBUG, "selectCat(unsigned) %i->SignalSelected()", selectedCat);
	categoryTabs[selectedCat].listView->SignalSelected();
}


void CategoryView::selectCat(Category cat) {
	for(unsigned int i=0; i < categoryList_s; i++) {
		if (categoryList[i] == cat) {
			selectCat(i);
		}
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
			log_printf(DBG_WARNING, "Touch in cat bar but couldn't find a matching category");
		} else {
			selectCat((unsigned) ind);
		}
	} else {
		if (input.KeyNewPressed(SCE_CTRL_LTRIGGER)) {
			selectPreviousCat();
			log_printf(DBG_DEBUG, "LTRIG, selectedCat: %d", selectedCat);
		}

		if (input.KeyNewPressed(SCE_CTRL_RTRIGGER)) {
			selectNextCat();
			log_printf(DBG_DEBUG, "RTRIG, selectedCat: %d", selectedCat);
		}

		if (input.KeyNewPressed(SCE_CTRL_TRIANGLE)) {
			log_printf(DBG_DEBUG, "TRIANGLE, start search");
			selectCat(SEARCH);
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

	if (selectedCat != activeCat && categoryTabs[selectedCat].listView->IsReadyToShow()) {
		log_printf(DBG_DEBUG, "New active tab: %d", activeCat);
		activeCat = selectedCat;
	}


	// FIXME focus is not a good solution
	categoryTabs[activeCat].listView->HandleInput(focus, input);

	return 0;
}

void CategoryView::selectNextCat() {
	int maxIndex = categoryList_s - 1;
	
	// Already at right-most
	if (activeCat >= maxIndex)
		return;
	
	if (categoryList[activeCat + 1] != SEARCH) {
		selectCat(activeCat + 1);
	// Jump to next next tab if it's next is search and search is not right-most
	} else if (activeCat + 2 <= maxIndex) {
		selectCat(activeCat + 2);
	}
}

void CategoryView::selectPreviousCat() {
	// Already at left-most
	if (activeCat == 0)
		return;
	
	if (categoryList[activeCat - 1] != SEARCH) {
		selectCat(activeCat - 1);
	// Jump to next previous tab if it's search and search is not left-most
	} else if (activeCat >= 2) {
		selectCat(activeCat - 2);
	} else {
		log_printf(DBG_DEBUG, "Ignored category selection from activeCat=%u", activeCat);
	}
}

int CategoryView::Display()
{
	categoryTabs[activeCat].listView->Display();

	img_catbar.Draw(Point(CAT_X, CAT_Y));

	for (unsigned int i=0; i < _countof(categoryList); i++) {
		if (categoryList_name[i][0] != '\0')
			font_33.DrawCentered(Rectangle(Point(categoryTabs[i].minX, CAT_Y), Point(categoryTabs[i].maxX, CAT_Y + CAT_HEIGHT)), categoryList_name[i]);
		if (categoryList[i] == SEARCH)
			img_magnifying_glass.DrawCentered(Point((categoryTabs[i].minX + categoryTabs[i].maxX)/2.0, CAT_Y + CAT_HEIGHT / 2.0));
		if (i > 0)
			img_catbar_sep.Draw(Point(categoryTabs[i].minX - 1, CAT_Y));
	}

	img_catbar_highlight.DrawResize(Point(CAT_X + categoryTabs[activeCat].minX, CAT_Y), Point(categoryTabs[activeCat].maxX - categoryTabs[activeCat].minX, CAT_HEIGHT));

	return 0;
}

