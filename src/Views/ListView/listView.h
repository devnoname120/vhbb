#pragma once

#include <global_include.h>

typedef struct {
	int posY;
	int selectedItem;
	int momentum;
	//Homebrew *hblist;
	int hblist_s;
} ListView;

int initListView();
int handleListViewInput(int focus, Input *input);
int displayListView();
