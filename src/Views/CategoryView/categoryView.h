#pragma once

#include <global_include.h>

#define CAT_X 0
#define CAT_Y 30

typedef enum {
	NEW,
	APPS,
	GAMES,
	EMULATORS,
	UTILITIES,
} Category;

int initCategoryView();
int handleCategoryViewInput(int focus, Input *input);
int displayCategoryView();
