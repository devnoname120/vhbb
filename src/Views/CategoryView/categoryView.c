typedef enum x {
	NEW,
	APPS,
	GAMES,
	EMULATORS,
	UTILITIES,
} Category;

const Category categoryList [] = {
	NEW,
	APPS,
	GAMES,
	EMULATORS,
	UTILITIES
	};

int selectedCat;

int initCategoryView()
{
	selectedCat = NEW;

	return 0;
}

int handleCategoryViewInput(int focus, Input *input)
{
	if (!focus)
		return 0;

	if 

}

int displayCategoryView()
{
	// TODO Display category background
	unsigned int count = countof(categoryList);


}