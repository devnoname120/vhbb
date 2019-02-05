#include <utility>

#pragma once

#include <global_include.h>

#include <Views/View.h>
#include <Views/IMEView.h>
#include <Views/ListView/listView.h>

#define CAT_X 0
#define CAT_Y 30
#define CAT_HEIGHT 49
#define CAT_AUTO_WIDTH -1

typedef enum {
	NEW,
	GAMES,
	PORTS,
	EMULATORS,
	UTILITIES,
	SEARCH
} Category;

struct CategoryTab {
	int minX;
	int maxX;
	ListView listView;

    explicit CategoryTab(ListView aListView) : listView(std::move(aListView)) {};
};

#define categoryList_s 6

class CategoryView: public View {
public:
	CategoryView();

	int HandleInput(int focus, const Input& input) override;
	int Display() override;

	
private:
	Font font_33;

	Texture img_catbar;
	Texture img_catbar_highlight;
	Texture img_catbar_sep;
	Texture img_magnifying_glass;

	unsigned int selectedCat;
	std::vector<CategoryTab> categoryTabs;

	int touchToCat(const Input &input);

	IMEViewResult _ime_search_view_result = {};
	void startSearch();
};

