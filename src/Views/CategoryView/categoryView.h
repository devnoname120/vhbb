#pragma once

#include <global_include.h>

#include <Views/View.h>
#include <Views/ListView/listView.h>

#define CAT_X 0
#define CAT_Y 30
#define CAT_HEIGHT 49

typedef enum {
	NEW,
	GAMES,
	PORTS,
	EMULATORS,
	UTILITIES,
} Category;

struct CategoryTab {
	int minX;
	int maxX;
	ListView listView;

	CategoryTab(ListView aListView) : listView(aListView) {};
};

#define categoryList_s 5

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
	
	unsigned int selectedCat;
	std::vector<CategoryTab> categoryTabs;

	int touchToCat(const Input &input);
};

