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

#define categoryList_s 5

int initCategoryView();
int handleCategoryViewInput(int focus, Input *input);
int displayCategoryView();
