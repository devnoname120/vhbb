#pragma once

#include <utility>

#include <Views/View.h>
#include <Views/IMEView.h>
#include <Views/ListView/listView.h>
#include <Views/ListView/searchView.h>

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
	std::unique_ptr<ListView> listView;

	explicit CategoryTab(std::unique_ptr<ListView> aListView, int minX, int maxX) : minX(minX), maxX(maxX), listView(std::move(aListView)) {};
	explicit CategoryTab(ListView *aListView, int minX = -1, int maxX = -1) : minX(minX), maxX(maxX), listView(aListView) {};
};

#define categoryList_s 6

class CategoryView: public View {
public:
	CategoryView();
	~CategoryView();

	int HandleInput(int focus, const Input& input) override;
	int Display() override;

	
private:
	Font font_33;

	Texture img_catbar;
	Texture img_catbar_highlight;
	Texture img_catbar_sep;
	Texture img_magnifying_glass;

	std::vector<CategoryTab> categoryTabs;

	unsigned int selectedCat = 0;
	unsigned int activeCat = 0;
	
	void selectPreviousCat();
	void selectNextCat();
	
	void selectCat(unsigned int cat);
	void selectCat(Category cat);

	unsigned int touchToCat(const Input &input, bool& notFound);
};

