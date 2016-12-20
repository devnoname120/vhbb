#pragma once

#include <global_include.h>

#define CAT_X 0
#define CAT_Y 30
#define CAT_HEIGHT 49

typedef enum {
	NEW,
	APPS,
	GAMES,
	EMULATORS,
	UTILITIES,
} Category;

typedef struct {
	int minX;
	int maxX;
} CategoryTab;

#define categoryList_s 5

class CategoryView: public View {
public:
	CategoryView();

	int HandleInput(int focus, const Input& input) override;
	int Display() override;

	
private:
	vita2d_texture *img_catbar;
	vita2d_texture *img_catbar_highlight;
	vita2d_texture *img_catbar_sep;

	vita2d_font *font_35;
	
	unsigned int selectedCat;
	// FIXME: Fixed size, not flexible
	std::vector<CategoryTab> categoryTabs;
	std::vector<ListView> listViews;
};

